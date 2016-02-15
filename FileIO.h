#include <string>
#ifndef FILEIO_FILEIO_H
#define FILEIO_FILEIO_H

using namespace std;

class FileIO {
public:
    FileIO(string a = "", int b = 0, int c = 0, string d = "");
    void setFileName(string name);
    void setStart(int s);
    void setEnd(int e);
    void setContent(string c);
    string getFileName();
    int getStart();
    int getEnd();
    string getContent();
    void Encode(FileIO F[], string fname);
    void Decode(string fname, FileIO F[]);

private:
    string filename;
    int start;
    int end;
    string content;

};


#endif //FILEIO_FILEIO_H
