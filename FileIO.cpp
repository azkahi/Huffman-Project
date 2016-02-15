FileIO::FileIO(string a, int b, int c, string d){
    filename = a;
    start = b;
    end = c;
    content = d;
}

void FileIO::setFileName(string name){ filename = name; }
void FileIO::setStart(int s){ start = s; }
void FileIO::setEnd(int e){ end = e; }
void FileIO::setContent(string c){ content = c; }
string FileIO::getFileName(){ return filename; }
int FileIO::getStart(){ return start; }
int FileIO::getEnd(){ return end; }
string FileIO::getContent(){ return content; }

void FileIO::Encode(FileIO F[], string fname){
    string content = "";
    int i = 0;
    int length = 0;
    char c;

    while (F[i].getFileName() != ""){
        ifstream infile(F[i].getFileName().c_str(), ios::in|ios::binary);
        while (infile.get(c)){
            content += c;
            //Algoritma Huffman taro disini
        }
        cout << content << endl;
        F[i].setContent(content);
        cout << F[i].getContent() << endl;
        content = "";
        i++;
        infile.close();
    }
    i = 0;
    ofstream ofile(fname.c_str(), ios::out|ios::binary);
    while (F[i].getContent() != ""){
        const char* name2 = F[i].getFileName().c_str();
        F[i].setStart(length);
        F[i].setEnd(length + F[i].getContent().length());
        ofile << F[i].getFileName() << " ";
        ofile << F[i].getStart() << " " << F[i].getEnd() << endl;
        i++;
    }
    ofile << batas << endl;
    i = 0;
    while (F[i].getContent() != ""){
        cout << F[i].getContent();
        ofile << F[i].getContent();
        i++;
    }
    ofile.close();
}

void FileIO::Decode(string fname, FileIO F[]){
    int i = 0;
    string name;
    int s, e;

    ifstream infile(fname.c_str(), ios::in|ios::binary);
    while (!infile.eof()){
        infile >> name >> s >> e;
        if (name == batas) break;
        F[i].setFileName(name);
        F[i].setStart(s);
        F[i].setEnd(e);
        i++;
    }
}
