#include "TP.h"

#include <iostream>                                                                                                                                                                                                                                                         
#include <TFile.h>
#include <TTree.h>

//int ParseArgs(int argc, const char** argv, std::string& inputFileList, TString& outputFile);
int ParseArgs(int argc, const char** argv, std::vector<std::string> files, std::vector<std::string> &nuana_files);

int main(int argc, char const* argv[]) {
  
  std::vector<std::string> files;
  std::vector<std::string> nuana_files;

  int parseArgResult = ParseArgs(argc, argv, files, nuana_files);
  if(parseArgResult != 0){
    return 1;
  }



  return 1;
}

//int ParseArgs(int argc, const char** argv, std::string& inputFileList, TString& outputFile){
int ParseArgs(int argc, const char** argv, std::vector<std::string> files, std::vector<std::string> &nuana_files) {

  int opt(1);
  while (opt < argc) {
    if (std::string(argv[opt]) == "--tpnf") { // two nu tp files
      files.push_back(std::string(argv[opt++]));
      files.push_back(std::string(argv[opt++]));
    } else if (std::string(argv[opt]) == "--tpcf") { // one cosmic
      files.push_back(std::string(argv[opt++]));
    } else if (std::string(argv[opt]) == "--annf") {
      nuana_files.push_back(std::string(argv[opt++]));
      nuana_files.push_back(std::string(argv[opt++]));
    } else {
      std::cout << "[ERROR] Unknown option." << std::endl;
      return 1;
    }
    opt++;
  }

  if (files.empty() || nuana_files.empty()) {
    std::cout << "[ERROR] Files not loaded." << std::endl;
    return 1;
  }

  return 0;

}

