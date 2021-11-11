#include <iostream>
#include <string>
#include <vector>
#include <fstream>

#include "hash.hpp"

class Transaction{
public:
    string transactionID;
    string from;
    string to;
    double Amount;

    Transaction(string sender, string receiver, double amount){
        from = sender;
        to = receiver;
        Amount = amount;

        Hash hash;

        transactionID = hash.generateHash(from + to + std::to_string(amount));
    }
};

class Block {
public:
    string blockHash;
    string previousHash;
    string timeStamp;
    string merkleHash;
    string version;
    int nonce;
    int difficulty;
    bool mined;
    bool transacted;
    std::vector<Transaction> transactions;

    Block(){}
    Block(string prevHash, string time, string v, int diff, std::vector<Transaction> t){
        previousHash = prevHash;
        timeStamp = time;
        version = v;
        difficulty = diff;
        transactions = t;

        Hash hash;

        std::vector<string> hashes;

        for (int i = 0; i < transactions.size(); i++){
            
            hashes.push_back(hash.generateHash(transactions[i].transactionID));
        }

        merkleHash = CreateMarkle(hashes);

        blockHash = hash.generateHash(hash.generateHash(previousHash + timeStamp + merkleHash) + std::to_string(nonce));
    }

    string CreateMarkle(std::vector<string> hashes){
        Hash hash;

        if (hashes.size() == 0)
            return "";

        if (hashes.size() == 1)
            return hashes[0];

        if (hashes.size() % 2 > 0)
            hashes.push_back(hashes[hashes.size() - 1]);

        std::vector<string> merkle;

        for (int i = 0; i < hashes.size(); i += 2)
        {
            string leafPair = hashes[i] + hashes[i + 1];
            merkle.push_back(hash.generateHash(hash.generateHash(leafPair)));
        }

        return CreateMarkle(merkle);
    }

    void Mine(){
        int currNonce = 0;
        Hash hash;
        string guessHash = hash.generateHash(hash.generateHash(previousHash + timeStamp + version + merkleHash) + std::to_string(currNonce));

        string hashToGuess = blockHash;
        for (int i = 0; i < difficulty; i++)
            hashToGuess[i] = '0';

        while (!mined)
        {
            if (guessHash.compare(hashToGuess) > 0)
            {
                currNonce++;
                guessHash = hash.generateHash(hash.generateHash(previousHash + timeStamp + version + merkleHash) + std::to_string(currNonce));
            }
            else
            {
                mined = true;
                nonce = currNonce;
            }
        }
    }
    
};

class User {
public:
    string Name;
    string PublicKey;
    double Balance;
};