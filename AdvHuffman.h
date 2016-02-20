#ifndef ADV_HUFFMAN_H
#define ADV_HUFFMAN_H

#include "Node.h"
#include <iostream>

using namespace std;

class AdvHuffman {
public:

    AdvHuffman();

    void Decoding(char* inputFile, char* outputFile);

    void Encoding(char* inputFile, char* outputFile);

    void Display(int T);

private :
    int* TabKar;
    /* Menyimpan alamat karakter yang ada pada TabNode*/
    Node* TabNode;
    /* Analogi pohon untuk Adaptive Huffman*/
    int* NomorNode;
    /* Berisi alamat dari pohon yang terurut dari bawah ke atas menaik serta dari kiri ke kanan*/
    int NYT;
    /* Menyimpan alamat NYK*/

    //METHOD FOR TABKAR
    bool searchChar(unsigned char K);
    /*Menghasilkan true ketika kar dberada pada pohon*/
    int adressChar(unsigned char K);
    /*Menghasilkan alamat kar pada pohon. -1 bila tidak ada*/
    void setAdrressChar(unsigned char K, int Address);
    /*Set array TabKar di indeks K dengann Address*/

    //METHOD FOR TABNODE
    void insertAfter(unsigned char K);
    /* Insert node dengan kar=K dan weight = 1 pada left NYK. Sedangkan right NYK diinsert node NYK baru*/
    void switchNode(int A, int B);
    /* Menukar 2 buah node pada pohon yang sama */
    char codeToChar(string s);
    /* convert barisan bit ke char */
    string charToCode(char c);
    /* convert c ke barisan bit */
    string charCodeFromTree(int T, unsigned char kar);
    /* menghasilkan barisan bit char dari pohon*/
    string NYTcodeFromTree(int T);
    /* menghasilkan barisan bit NYT dari pohon*/
    bool searchCharTree(int T, unsigned char c);
    /* menghasilkan true jika c berada di pohon*/
    bool searchNYTTree(int T);
    /* menghasilkan true jika NYT berada di pohon*/


    //SUPPORTED METHOD
    bool isLeaf(int T);
    /* mengembalikan true jika T adalah daun */
    bool isLeft(int T);
    /* mengembalikan true jika T merupakan anak kiri dari parent-nya*/
    bool isRight(int T);
    /* mengembalikan true jika T merupakan anak kana dari parent-nya*/

    //SUPPORTED METHOD FOR ADAPTIVE HUFFMAN
    void Update(int T, bool baru);
    /* Update pohon saat menginput node baru. T merupakan node baru yang diinput atau frequensi node lama yang ditambah*/

    //SUPPORTED METHOD FOR UPDATE()
    void UpdateNomorNode();
    /* Mengurutkan isi NomorNode */
    int MaxNumberNodeInBlock(int P);
    /* Mencari nomor node maksimum yang memiliki weight yang sama dengan P */

    //SUPPORTED METHOD FOR UPDATE NUMBER
    int DeepTree(int T);
    /* Menghasilkan nilai dari kedalaman sebuah pohon*/
    void setNomorNodeLevel(int T, int N, int *StartNumber);
    /* Menginput alamat node ke NomorNode terurut membesar dari kiri ke kana*/

    void BarisBin(char* inputFile, char* outputFile);

};

#endif // ADV_HUFFMAN_H
