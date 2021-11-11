#include "main.hpp"

std::string gen_random(const int len) {
    static const char alphanum[] =
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";
    std::string tmp_s;
    tmp_s.reserve(len);

    for (int i = 0; i < len; ++i) {
        tmp_s += alphanum[rand() % (sizeof(alphanum) - 1)];
    }
    
    return tmp_s;
}

double fRand(double fMin, double fMax)
{
    double f = (double)rand() / RAND_MAX;
    return fMin + f * (fMax - fMin);
}

std::vector<User> GenerateUsers(int n){
    std::vector<User> users;
    Hash hash;
    srand (time(NULL));

    for(int i = 0; i < n; i++){
        User u;
        u.Name = std::to_string(i);
        u.PublicKey = hash.generateHash(gen_random(rand() % 512 + 1));
        u.Balance = fRand(100, 1000000);
        
        users.push_back(u);
    }

    return users;
}

std::vector<Transaction> GenerateTransactions(int n, std::vector<User> users){
    std::vector<Transaction> transactions;
    srand (time(NULL));

    for(int i = 0; i < n; i++){
        string sender = users[rand() % users.size()].PublicKey;
        string receiver = users[rand() % users.size()].PublicKey;
        double amount = fRand(100, 1000);
        Transaction t(sender, receiver, amount);

        transactions.push_back(t);
    }

    return transactions;
}

std::string currentDateTime() {
    time_t     now = time(0);
    struct tm  tstruct;
    char       buf[80];
    tstruct = *localtime(&now);

    strftime(buf, sizeof(buf), "%Y-%m-%d.%X", &tstruct);

    return buf;
}

int findIndex(const vector<User> &arr, string key) {

    for (auto i = 0; i < arr.size(); ++i) {
        if (arr[i].PublicKey == key)
            return i;
    }

    return -1;
}

void deleteValues(std::vector<Transaction> &v1, std::vector<string> v2){
    for(int i = 0; i < v1.size(); i++){
        for(int j = 0; j < v2.size(); j++){
            if(v1[i].transactionID == v2[j]){
                v1.erase(v1.begin() + i);
            }
        }
    }
}

int main(){

    Hash hash;
    ofstream transactionsFile, blocksFile;
    transactionsFile.open("transactions.txt");
    blocksFile.open("blocks.txt");

    std::cout<<"Data generation.. \n";

    std::vector<User> users = GenerateUsers(1000);    
    std::vector<Transaction> transactions = GenerateTransactions(10000, users);
    std::vector<Block> blocks;

    std::cout << "Starting mining.. \n";

    while(transactions.size() > 0){

        std::vector<Block> candidates;
        
        for(int i = 0; i < 3; i++){
            string previousHash;

            if(blocks.size() == 0){
                previousHash = "0000000000000000000000000000000000000000000000000000000000000000";
            }
            else{
                previousHash = blocks[blocks.size() - 1].blockHash;
            }

            int count;

            if(transactions.size() >= 1000){
                count = rand() % 1000 + 1;
            }
            else{
                count = transactions.size();
            }

            std::vector<Transaction>::const_iterator first = transactions.begin();
            std::vector<Transaction>::const_iterator last = transactions.begin() + count;
            std::vector<Transaction> vec(first, last);

            Block b(previousHash, currentDateTime(), "1", 1, vec);
            candidates.push_back(b);
        }

        Block newBlock;

        for(auto candidate : candidates){
            candidate.Mine();

            for(auto c : candidates){
                if(c.mined){
                    newBlock = c;
                    break;
                }
            }
        }

        int transactionCount = 1;

        if(newBlock.mined) {
            std::cout << "Block mined! \n";
            std::vector<string> general;
            std::vector<string> invalid;

            for(auto t : transactions) {
                int sender = findIndex(users, t.from);
                int receiver = findIndex(users, t.to);

                if(hash.generateHash(t.from + t.to + std::to_string(t.Amount)) == t.transactionID && users[sender].Balance >= t.Amount){
                    bool transacted = false;

                    if(users[sender].Balance >= t.Amount){
                        transacted = true;
                        users[receiver].Balance += t.Amount;
                        users[sender].Balance -= t.Amount;
                    }

                    if(transacted){
                        transactionsFile << "#" << transactionCount << " Sender: " << users[sender].PublicKey << " Receiver: " << users[receiver].PublicKey << " Amount: " << t.Amount << endl;
                        transactionCount++;

                    }

                }
                else {
                    invalid.push_back(t.transactionID);
                }
                general.push_back(t.transactionID);
            }

            deleteValues(transactions, general);
            deleteValues(newBlock.transactions, invalid);

            if(transactionCount != 1){
                newBlock.transacted = true;
            }
        }

        transactionCount--;
        blocks.push_back(newBlock);

        blocksFile << "Hash: " << newBlock.blockHash << "\nPrevious hash: " << newBlock.previousHash << "\nTimestamp: " << newBlock.timeStamp << "\nDifficulty: " << newBlock.difficulty << "\nMerkle root:" << newBlock.merkleHash << "\nNonce: " << newBlock.nonce << "\n\n";

    }

    std::cout<< "Results have been written to files";

    return 0;
}