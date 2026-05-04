#include <stdio.h>
#include <string.h>
#include <winsock2.h>

#define SIZE 1000

int main() {
    
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2,2), &wsaData);//소켓 라이브러리 초기화

    SOCKET server_socket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);//TCP 선택하여 소켓 생성, 접속유무 판단 소켓
    
    SOCKADDR_IN server_addr;
    ZeroMemory(&server_addr, sizeof(server_addr));//0으로 초기화
    
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(ADDR_ANY);//허용할 클라이언트의 주소
    server_addr.sin_port = htons(5000);//포트넘버
    bind(server_socket, (SOCKADDR *) &server_addr, sizeof(server_addr));//소켓에 열어놓을 주소 포트 초기화

    listen(server_socket, SOMAXCONN);//대기줄 길이 최대

    while(TRUE){
        printf("Waiting for client connection.\n");
        
        SOCKET client_socket = accept(server_socket, NULL, NULL);//실제 통신할 소켓 생성
    
        if(client_socket == INVALID_SOCKET){
            printf("Client connected fail. \n");
            continue;//연결실패시 다시 다른 클라이언트 요청을 처리
        }
        else printf("Client connected. \n");

        char recv_message[SIZE];
        
        while(TRUE){
            int recv_len = recv(client_socket, recv_message, sizeof(recv_message) - 1, 0);
            
            int disconnect = 0;
            if(recv_len == SOCKET_ERROR || recv_len == disconnect) {
                printf("Client disconnected. \n");
                
                break;
            }
            
            int connect_close = 0; 
            recv_message[recv_len] = '\0';
            if((strcmp(recv_message, "quit")&&strcmp(recv_message, "QUIT")) == connect_close) break;//탈출후 소켓 닫고 다음 연결시도

            printf("Received from client : %s \n", recv_message);

            //에코 서버라서 클라이언트가 보낸데이터 그대로
            send(client_socket, recv_message, (int)strlen(recv_message), 0);
        }
        closesocket(client_socket);//현재연결 소켓 닫기 - 다음 클라이언트 통신 확인후 다시 통신 
    }

    //서버는 항상 켜져있음
    
    return 0;
}