#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <vector>
#include "clientSocket.h"

using namespace std;


// Need to link with Ws2_32.lib, Mswsock.lib, and Advapi32.lib
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")


#define DEFAULT_PORT_CLIENT "27016"
#define MATRIX_SIZE 256


class FullImageMatrix {

private:
    int sumMetrix;
    short* shortMat;
    static const int mult = 1000000;

public:

    FullImageMatrix(short* mat, int sum) {
        shortMat = mat;
        sumMetrix = sum;
    }

    ~FullImageMatrix() {
        delete[] shortMat;
    }

    double getNumByIndex(int i, int j) {
        //this fun is not in use, just show the way to access the matrix
        return double(shortMat[i * MATRIX_SIZE + j]) / sumMetrix;
    }

    void printMat() {
        for (int i = 0; i < MATRIX_SIZE ; i++) {
            cout << "line " << i << endl;
            for (int j = 0; j < MATRIX_SIZE; j++) {
                short x = shortMat[i * MATRIX_SIZE + j];
                cout << double(x) / sumMetrix << " ";
            }
            cout << endl;
        //It may be shorter, but it use a lot of memory
        //double m[MATRIX_SIZE * MATRIX_SIZE];
        //for (int i = 0; i < MATRIX_SIZE * MATRIX_SIZE; i++) {
        //    m[i] = double(shortMat[i]) / sumMetrix;
        //}
        //for (int i = 0; i < MATRIX_SIZE; i++) {
        //    cout << "line " << i << endl;
        //    for (int j = 0; j < MATRIX_SIZE; j++) {
        //        cout << m[i * MATRIX_SIZE + j] << " ";
        //    }
        //    cout << endl;
        //}
        }
    }


};

short* recvMat(clientSocket* sock) {
    int bufSize = MATRIX_SIZE * MATRIX_SIZE * sizeof(short);
    char* recvbufMat = new char[bufSize];
    int iResult = sock->receiveData(recvbufMat, bufSize);
    if (iResult <= 0) {
        delete[] recvbufMat;
        return nullptr;
    }
    cout << ((short*)recvbufMat)[0] << endl;

    return (short*)recvbufMat;
}

int recvSum(clientSocket* sock) {
    int bufSize = sizeof(int);
    char* recvbufSum = new char[bufSize];
    int iResult = sock->receiveData(recvbufSum, bufSize);
    if (iResult <= 0) {
        delete[] recvbufSum;
        return -1;
    }
    cout << ((int*)recvbufSum)[0] << endl;
    return ((int*)recvbufSum)[0];
}

int __cdecl main(int argc, char** argv)
{
    clientSocket* cliSock = new clientSocket(DEFAULT_PORT_CLIENT);
    // Receive until the peer closes the connection
    while (true) {
        short* mat = recvMat(cliSock);
        if (!mat)
            break;
        int sum = recvSum(cliSock);
        if (sum <= 0)
            break;
        FullImageMatrix* matrix = new FullImageMatrix(mat,sum);

        matrix->printMat();


        delete matrix;

    };

    cliSock->shutdownConnection();

    return 0;
}