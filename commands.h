#ifndef COMMANDS_H_
#define COMMANDS_H_


#include <iomanip>
#include <iostream>
#include <string.h>
#include <ostream>
#include <fstream>
#include <vector>
#include "HybridAnomalyDetector.h"

using namespace std;

class DefaultIO{
public:
	virtual string read()=0;
	virtual void write(string text)=0;
	virtual void write(float f)=0;
	virtual void read(float* f)=0;
	virtual ~DefaultIO(){}

	// you may add additional methods here
};
// you may add here helper classes
 class DataMenager {
 	HybridAnomalyDetector* ad;
	vector<AnomalyReport> ar;
	int rows; //n
	bool is_menu_running = true;
	 public:
	 	DataMenager() {
			 this->ad = new HybridAnomalyDetector();
		 }
		~DataMenager() {
			free(this->ad);
		}
		float get_current_Threshhold() { return this->ad->getThreshhold();}
		void set_new_Threshhold(float t) { this->ad->setCorrelationThreshold(t);}
		HybridAnomalyDetector* getDetector() {return this->ad;}
		vector<AnomalyReport> getReports() {return this->ar;}
		void set_anomaly_reports(vector<AnomalyReport> ar1) { 
			for (int i = 0; i < ar1.size(); i++) {
			this->ar.push_back(ar1[i]);
		}
			}
		bool is_running() { return this->is_menu_running;}
		void exitMenu() {this->is_menu_running = false;}
		void setRows(int x) { this->rows = x;}
		int getRows() { return this->rows; }
};

// you may edit this class
class Command{
	protected:
		DefaultIO* dio;
		DataMenager* dm;
		string description;
	public:
		Command(DefaultIO* dio, DataMenager* dm):dio(dio), dm(dm){}
		virtual string getDescription() {return this->description;}
		virtual void execute()=0;
		virtual ~Command(){}
};

// implement here your command classes
class uploadCommand: public Command {
public:
	uploadCommand(DefaultIO* dio, DataMenager* dm):Command(dio, dm){
		this->description = "1.upload a time series csv file\n";
	}
	void execute() override {
		this->dio->write("Please upload your local train CSV file.\n");
		// read csv file
		readCSV("anomalyTrain.csv");
		this->dio->write("Upload complete.\n");
		this->dio->write("Please upload your local test CSV file.\n");
		readCSV("anomalyTest.csv");
		// read cs file
		this->dio->write("Upload complete.\n");
	}
private:
	void readCSV(const string& fileName) {
		std::ofstream file;
    	file.open(fileName);
		string line = this->dio->read() + '\n';
		while(line != "done\n") {
			file << line;
			line = this->dio->read() + '\n';

		}
		file.close();
	}
};
class algoSettings: public Command {
	
public:
	algoSettings(DefaultIO* dio, DataMenager* dm):Command(dio, dm){
		 this->description = "2.algorithm settings\n";
	}
	void execute() override {
		float thresh = this->dm->get_current_Threshhold();
		// convert float to string
		string s_thresh(to_string(thresh));
		thresh = floor(thresh * 10000) / 10000;
		string t = to_string(thresh);
		this->dio->write("The current correlation threshold is " + t + "\n");
		//thresh = floor(thresh * 10000) / 10000;
		//this->dio->write(thresh);
		//this->dio->write("\n");
		this->dio->write("Type a new threshold\n");
		string new_thresh = this->dio->read();
		float new_threshhold = stof(new_thresh);
		while (new_threshhold < 0 || new_threshhold > 1) {
			this->dio->write("please choose a value between 0 and 1.\n");
			new_thresh = this->dio->read();
			new_threshhold = stof(new_thresh);
		}
		this->dm->set_new_Threshhold(new_threshhold);
	}
};
class detectCommand: public Command {
	
public:
	detectCommand(DefaultIO* dio, DataMenager* dm):Command(dio, dm){
		this->description = "3.detect anomalies\n";
	}
	void execute() override {
		TimeSeries train("anomalyTrain.csv");
		TimeSeries test("anomalyTest.csv");
		this->dm->getDetector()->learnNormal(train);
		vector<AnomalyReport> ar2 = this->dm->getDetector()->detect(test);
		this->dm->setRows(test.getCol(test.getKeys()[0]).size());
		this->dm->set_anomaly_reports(ar2);
		this->dio->write("anomaly detection complete.\n");

	}
};
class displayCommand: public Command {
	
public:
	displayCommand(DefaultIO* dio, DataMenager* dm):Command(dio, dm){
		this->description = "4.display results\n";
	}
	void execute() override {
		vector<AnomalyReport> r = this->dm->getReports();
		// sort ar
		// write ar
		int i;
		string time_step;
		int size = r.size();
		for (i = 0; i < size; i++) {
			time_step = to_string(r[i].timeStep);
			this->dio->write(time_step +" " +	"	" + r[i].description + "\n");
		}
		this->dio->write("Done.\n");

	}

};
class analyzeCommand: public Command {
public:
	analyzeCommand(DefaultIO* dio, DataMenager* dm):Command(dio, dm){
		this->description = "5.upload anomalies and analyze results\n";
	}
	void execute() override{
		vector<pair<int,int>> client_input;
		vector<pair<int,int>> intersect;
		this->dio->write("Please upload your local anomalies file.\n");
		string line = this->dio->read();
		while(line != "done") {
			string first, second;
			stringstream ss(line);
			getline(ss, first, ',');
			getline(ss, second ,',');
			//pair{stoi(first), stoi(second)}
			client_input.push_back({stoi(first), stoi(second)});
			line = this->dio->read();
		}
		this->dio->write("Upload complete.\n");
		//intersect all of anomalies with same description 
		vector<AnomalyReport> myReports = this->dm->getReports();
		int size_of_reports =  myReports.size();
		int i, start, end, j;
		for (i = 0; i < size_of_reports - 1; i++) {
			start = i;
			end = start;
			while(myReports[i].description == myReports[i + 1].description &&
			 (myReports[i].timeStep + 1) == myReports[i + 1].timeStep) {
				end++;
				i++;
				if (i == size_of_reports) {
					break;
				}
			}
			if (start != end) {
				intersect.push_back({myReports[start].timeStep, myReports[end].timeStep});
			}
		}
		int Positive = client_input.size();
		int sum_anomalies = 0;
		for (i = 0; i < client_input.size(); i++) {
			sum_anomalies += (client_input[i].second - client_input[i].first) + 1;
		}
		int Negative = this->dm->getRows() - sum_anomalies;
		int False_Negative = 0, True_Positive = 0, False_Positive = 0;
		bool is_true_positive = false;
		for(i = 0; i < intersect.size(); i++) {
			for(j = 0; j < Positive; j++) {
				if ((intersect[i].first <= client_input[j].first && intersect[i].second >= client_input[j].first)
				|| (client_input[j].first <= intersect[i].first  && client_input[j].second >= intersect[i].first)) {
					True_Positive++;
					is_true_positive = true;
				}
	
			}
			if (!is_true_positive) 
				False_Positive++;
			is_true_positive = false;
		}	
		//False_Negative = Positive - True_Positive;
		double TPR = (double)True_Positive / (double)Positive;
		double FAR =  (double)False_Positive / double(Negative);
		TPR = floor(TPR * 1000) / 1000;
		FAR = floor(FAR * 1000) / 1000;
		//string tpr = to_string(TPR);
		//string far = to_string(FAR);
		std::ostringstream ss;
		TPR = floor(TPR * 1000) / 1000;
		ss <<  TPR;
		string s_tpr = ss.str();
		this->dio->write("True Positive Rate: " + s_tpr + "\n");
		std::ostringstream ss1;
		FAR = floor(FAR * 1000) / 1000;
		ss1 <<  FAR;
		string s_far = ss1.str();
		this->dio->write("False Positive Rate: " + s_far + "\n");
	
	
	}
};
class exitCommand: public Command {
public:
	exitCommand(DefaultIO* dio, DataMenager* dm):Command(dio, dm){
		this->description = "6.exit\n";
	}
	void execute() override{
		this->dm->exitMenu();
	}
};

#endif /* COMMANDS_H_ */
