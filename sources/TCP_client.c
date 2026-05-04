#include <stdio.h>
#include <string.h>
#include <winsock2.h>

#define SIZE 1000

int main() {
    
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2,2), &wsaData);//소켓 라이브러리 초기화

    SOCKET client_socket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);//TCP 선택하여 소켓 생성
    
    SOCKADDR_IN server_addr;
    ZeroMemory(&server_addr, sizeof(server_addr));//0으로 초기화
    
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);//접속할 서버의 주소(하나의 컴퓨터에서 서버 클라이언트 모두 띄워 통신)
    server_addr.sin_port = htons(5000);//포트넘버

    int check_connect;
    check_connect = connect(client_socket, (SOCKADDR *) &server_addr, sizeof(server_addr));//server에게 연결 요청보내기, SOCKADDR 공용구조체, 읽을 크기

    if(check_connect == SOCKET_ERROR){
        printf("Connection failed.\n");
        
        return 0;
    }
    else printf("Connected to the server.\n");

    char send_message[SIZE];
    char recv_message[SIZE];
        
    while(TRUE){
        int delete_newlinechar = 0;
        printf("Enter message to send. (Stop connect is 'quit'): ");
        fgets(send_message, SIZE, stdin);
        send_message[strcspn(send_message, "\n")] = delete_newlinechar;

        //입력한 메세지 보내기
        send(client_socket, send_message, (int)strlen(send_message), 0);

        int connect_close = 0;
        if((strcmp(send_message, "quit")&&strcmp(send_message, "QUIT")) == connect_close) break;//탈출후 소켓 닫고 함수 종료
        
        int recv_len = recv(client_socket, recv_message, sizeof(recv_message) - 1, 0);//문자 데이터 출력이기에 -1로 끝문자
        
        int disconnect = 0;
        if(recv_len == SOCKET_ERROR || recv_len == disconnect) {
            printf("Server disconnected. \n");
                
            break;
        }
        
        recv_message[recv_len] = '\0';
        printf("Received from server : %s \n", recv_message);
    }

    closesocket(client_socket);//소켓 닫기

    WSACleanup();//소켓 라이브러리 해제
    
    return 0;
}