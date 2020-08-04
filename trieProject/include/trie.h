#ifndef TRIE_H
#define TRIE_H

#include "TrieNode.h"
#include "trieNodeFull.h"
#include <string>
#include <stdexcept>

using std::runtime_error;
using std::string;

class Trie
{
private:
    TrieNode *root;

    void clearAux(TrieNode *current)
    {
        List<TrieNode*> *pointers = current->getChildrenPointers();
        for (pointers->goToStart(); !pointers->atEnd(); pointers->next())
            clearAux(pointers->getElement());
        delete current;
    }
    void getMatchesAux(TrieNode* current, string prefix, List<string> *words)
    {
        if (current->getIsFinal())
            words->append(prefix);
        List<char> *children = current->getChildren();
        for (children->goToStart(); !children->atEnd(); children->next())
        {
            char c = children->getElement();
            string newPrefix = prefix;
            newPrefix.append(1, c);
            getMatchesAux(current->getChild(c), newPrefix, words);
        }
        delete children;
    }
public:
    Trie()
    {
        root = new TrieNode();
    }
    ~Trie()
    {
        clear();
        delete root;
    }

    void insert(int c, string word)
    {
        if (!containsWord(word))
        {
            TrieNode *current = root;
            //cout << endl << "size=" << word.size() << " ";
            for (unsigned int i = 0; i < word.size(); i++)
            {
                //cout << " index=" << i << " ";
                current->increaseCount();
                if (!current->contains(word[i]))
                {
                    /*if(i == word.size()-1)  //Solo entra si es la �ltima letra
                    {
                        cout << i << "=" << word;
                        TrieNodeFull *newNode = new TrieNodeFull();
                    }
                    else
                    {
                        TrieNode *newNode = new TrieNode();
                    }*/
                    TrieNode *newNode = new TrieNodeFull();
                    current->add(word[i], newNode);
                }
                current = current->getChild(word[i]);

            }
            current->increaseCount();
            current->setIsFinal(true);
            //cout << c << " " << word << endl;
            current->setIndex(c);
        }

    }

    bool containsWord(string word)
    {
        TrieNode* current = root;
        for (unsigned int i = 0; i < word.size(); i++)
        {
            if (!current->contains(word[i]))
                return false;
            current = current->getChild(word[i]);
        }
        return current->getIsFinal();

    }

    bool containsPrefix(string prefix)
    {
        TrieNode* current = root;
        for (unsigned int i = 0; i < prefix.size(); i++)
        {
            if (!current->contains(prefix[i]))
                return false;
            current = current->getChild(prefix[i]);
        }
        return true;
    }

    int prefixCount(string prefix)
    {
        TrieNode* current = root;
        for (unsigned int i = 0; i < prefix.size(); i++)
        {
            if (!current->contains(prefix[i]))
                return 0;
            current = current->getChild(prefix[i]);
        }
        return current->getPrefixCount();
    }

    void remove(string word)
    {
        if (!containsWord(word))
            throw runtime_error("Word not found.");
        TrieNode *current = root;
        for (unsigned int i = 0; i < word.size(); i++)
        {
            current->decreaseCount();
            TrieNode *child = current->getChild(word[i]);
            if (current->getPrefixCount() == 0)
                delete current;
            else if (child->getPrefixCount() == 1)
                current->remove(word[i]);
            current = child;
        }
        current->decreaseCount();
        if (current->getPrefixCount() == 0)
            delete current;
        else
            current->setIsFinal(false);
    }

    void clear()
    {
        clearAux(root);
        root = new TrieNode();
    }

    List<string>* getMatches(string prefix)
    {
        List<string> *words = new DLinkedList<string>();
        TrieNode* current = root;
        for (unsigned int i = 0; i < prefix.size(); i++)
        {
            if (!current->contains(prefix[i]))
                return words;
            else
                current = current->getChild(prefix[i]);
        }
        getMatchesAux(current, prefix, words);
        return words;
    }

    /*int getLines(string word)
    {
        if (!containsWord(word))
            throw runtime_error("Word not found.");
        TrieNode* current = root;
        for (unsigned int i = 0; i < word.size(); i++)
        {
            if (!current->contains(word[i]))
                return 0;
            current = current->getChild(word[i]);
        }
        current->getIndex();
    }

    int getLinesSize(string word)
    {
        if (!containsWord(word))
            throw runtime_error("Word not found.");
        TrieNode* current = root;
        for (unsigned int i = 0; i < word.size(); i++)
        {
            if (!current->contains(word[i]))
                return 0;
            current = current->getChild(word[i]);
        }
        current->getIndex()->print();
        return current->getIndexSize();

    }*/

};

#endif // TRIE_H
