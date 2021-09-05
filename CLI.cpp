#include "CLI.h"

CLI::CLI(DefaultIO* dio) {
    this->dio = dio;
}

void CLI::start(){
    string s1 = "Welcome to the Anomaly Detection Server.\n";
    string s2 = "Please choose an option:\n";
    this->dm = new DataMenager();
    uploadCommand c1 = uploadCommand(this->dio, this->dm);
    algoSettings c2 = algoSettings(this->dio, this->dm);
    detectCommand c3 = detectCommand(this->dio, this->dm);
    displayCommand c4 = displayCommand(this->dio, this->dm);
    analyzeCommand c5 = analyzeCommand(this->dio, this->dm);
    exitCommand c6 = exitCommand(this->dio, this->dm);
    
    this->commands.push_back(&c1);
    this->commands.push_back(&c2);
    this->commands.push_back(&c3);
    this->commands.push_back(&c4);
    this->commands.push_back(&c5);
    this->commands.push_back(&c6);

    while(this->dm->is_running()) {
        this->dio->write(s1);
        this->dio->write(s2);
        int j;
        for(j = 0; j < 6; j++) {
            this->dio->write(commands[j]->getDescription());
        }
         //float* f;
         string nextLine = this->dio->read();
         int c = stoi(nextLine);
        commands[c - 1]->execute();
    }
}


CLI::~CLI() {
    free(this->dm);
}

