#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <filesystem>
using namespace std;

void SplitString(const string &s, vector<string> &v, const string &c) {
  string::size_type pos1, pos2;
  pos2 = s.find(c);
  pos1 = 0;
  while (string::npos != pos2) {
    if (pos1 != pos2)
      v.push_back(s.substr(pos1, pos2 - pos1));

    pos1 = pos2 + c.size();
    pos2 = s.find(c, pos1);
  }
  if (pos1 != s.length())
    v.push_back(s.substr(pos1));
}


void my_command(string input_name){
  //system("copy myfile.txt + edge.txt myfile.txt");
  system(("COPY "+input_name+"_assist.txt + "+input_name+".txt "+input_name+"_assist.txt").c_str());
  cout<<("COPY "+input_name+"_assist.txt + "+input_name+".txt "+input_name+"_assist.txt").c_str()<<endl;
  system(("DEL "+input_name+".txt").c_str());
  system(("RENAME "+input_name+"_assist.txt "+input_name+".txt").c_str());
}  


void transform_emu (string tsv_filename){
  fstream inputfile(tsv_filename+".tsv");
  vector<string> Q;
  vector<string> energy;

  if (!inputfile.is_open()) {
    cout << "cannot open the file" << endl;
    return ;
  }

  ofstream outputfile("./"+tsv_filename+".txt", ios::app); // app表示每次操作前均定位到文件末尾
  if (outputfile.fail()) {
    cout << "error\n";
  }

  string s;
  getline(inputfile,s);
  getline(inputfile,s);

  while(getline(inputfile,s)){
    if (s==" ") continue;
    vector<string> vec;
    SplitString(s, vec, "\t");

    if (vec[0] == "Q") {
      Q.push_back(vec[1]);
      outputfile << "# Group " << Q.size()-1 << "\n";
    } // write here
    else {
      if (Q.size() == 2) {
        energy.push_back(vec[0]);
      }
      outputfile << vec[1] << " " << vec[2] << "\n";       
    }
  }
  inputfile.close();
  outputfile.close();
  ofstream assist_output("./"+tsv_filename+"_assist.txt");
  
  assist_output<<"# Number of Energy transfer values\n";
  assist_output<<energy.size()<<"\n";
  assist_output<<"# Number of Y values\n";
  assist_output<<Q.size()<<"\n";
  assist_output<<"# Energy transfer (meV) values\n";
  for(auto i:energy) assist_output<<i<<"\n";
  assist_output<<"# Y () values\n";
  for(int i=0;i<Q.size();++i) assist_output<<i<<"\n";
  assist_output.close();
  my_command(tsv_filename);
}


int main(int argc, const char *argv[]) {
  filesystem::path tsv_path(".");
  if (!filesystem::exists(tsv_path)) return 1;
  filesystem::directory_entry entry(tsv_path);
  if (entry.status().type()==filesystem::file_type::directory) cout<<"At least, this is a directory!"<<endl;
  filesystem::directory_iterator path_iter(tsv_path);

  system("MKDIR txtOutput");
  for (auto& single_file:path_iter){
    if (single_file.path().extension().string()==".tsv")  {
      string single_name(single_file.path().stem().string()) ;
      transform_emu(single_name);
      system(("MOVE "+single_name+".txt txtOutput").c_str());
    }
  }
  cout << "FINISH" << endl;
  return 0;
}
