#include <stdio.h>
#include <string.h>
#include <winsock2.h>
#include <windows.h> //전송 속도를 제한하기 위해 프로세스를 멈추는 Sleep()함수 활용
#include <time.h>    //throuput 측정용 ms측정을 위해 clock()함수

int UDP_client(int speed_select); // UDP 클라이언트 함수, 전송 속도 조절가능
int TCP_client(int speed_select); // TCP 클라이언트 함수, 전송 속도 조절가능

int main()
{
    int TCP_UDP_mode, speed_select;
    int speed[3] = {500, 1000, 2000};

    while (TRUE)
    {
        printf("Select TCP or UDP (TCP is '1', UDP is '2', Quit is '3') : ");
        scanf("%d", &TCP_UDP_mode);

        if (TCP_UDP_mode == 3)
            break;

        printf("Select transport speed (1. 500bytes/sec 2. 1000bytes/sec 3. 2000bytes/sec) : ");
        scanf("%d", &speed_select);

        if (TCP_UDP_mode == 1)
            TCP_client(speed[speed_select - 1]);
        if (TCP_UDP_mode == 2)
            UDP_client(speed[speed_select - 1]);
    }

    return 0;
}

int TCP_client(int speed_select)
{

    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData); // 소켓 라이브러리 초기화

    SOCKET client_socket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP); // TCP 선택하여 소켓 생성

    SOCKADDR_IN server_addr;
    ZeroMemory(&server_addr, sizeof(server_addr)); // 0으로 초기화

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK); // 접속할 서버의 주소(하나의 컴퓨터에서 서버 클라이언트 모두 띄워 통신)
    server_addr.sin_port = htons(5000);                   // 포트넘버

    int check_connect;
    check_connect = connect(client_socket, (SOCKADDR *)&server_addr, sizeof(server_addr)); // server에게 연결 요청보내기, SOCKADDR 공용구조체, 읽을 크기

    if (check_connect == SOCKET_ERROR)
    {
        printf("Connection failed.\n");

        return 0;
    }
    else
        printf("Connected to the server.\n");

    char send_message[speed_select];
    memset(send_message, 'T', speed_select); // 더미 데이터채우기

    printf("Send dummy data (speed %d bytes/sec) to the server during 5 second. \n", speed_select);

    clock_t start_time, end_time;
    double total_second; // 총 걸린 시간(단위 second)
    int total_byte = 0;  // 총 데이터 크기(단위 byte)
    int send_len;        // 보내는데 성공한 데이터(단위 byte)

    start_time = clock(); // 데이터 보내기전 측정 시작

    for (int i = 0; i < 5; i++)
    { // 총 5초동안 전송
        // 입력한 메세지 보내기
        send_len = send(client_socket, send_message, speed_select, 0);

        if (send_len != SOCKET_ERROR)
            total_byte += send_len;

        Sleep(1000); // 1초당 선택된 전송 속도만큼 보냄
    }

    end_time = clock();

    total_second = (double)(end_time - start_time) / CLOCKS_PER_SEC; // second 초 단위로 변환

    printf("Compelete send dummy data. \n");

    printf("TCP throuput : %f byte/sec \n", total_byte / total_second);

    send(client_socket, "QUIT", 4, 0); // TCP 전송 종료 신호

    closesocket(client_socket); // 소켓 닫기

    WSACleanup(); // 소켓 라이브러리 해제

    return 0;
}

int UDP_client(int speed_select)
{

    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData); // 소켓 라이브러리 초기화

    SOCKET client_socket = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP); // UDP 선택하여 소켓 생성

    SOCKADDR_IN server_addr;
    ZeroMemory(&server_addr, sizeof(server_addr)); // 0으로 초기화

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK); // 접속할 서버의 주소(하나의 컴퓨터에서 서버 클라이언트 모두 띄워 통신)
    server_addr.sin_port = htons(5000);                   // 포트넘버

    char send_message[speed_select];
    memset(send_message, 'U', speed_select); // 더미 데이터채우기

    printf("Send dummy data (speed %d bytes/sec) to the server during 5 second. \n", speed_select);

    clock_t start_time, end_time;
    double total_second; // 총 걸린 시간(단위 second)
    int total_byte = 0;  // 총 데이터 크기(단위 byte)
    int send_len;        // 보내는데 성공한 데이터(단위 byte)

    start_time = clock(); // 데이터 보내기전 측정 시작

    for (int i = 0; i < 5; i++)
    { // 총 5초동안 전송
        // 입력한 메세지 보내기, 연결안되어있어서 메세지 + 서버주소포트번호 넣어주기
        send_len = sendto(client_socket, send_message, speed_select, 0, (SOCKADDR *)&server_addr, sizeof(server_addr));

        if (send_len != SOCKET_ERROR)
            total_byte += send_len;

        Sleep(1000); // 1초당 선택된 전송 속도만큼 보냄
    }

    end_time = clock();

    total_second = (double)(end_time - start_time) / CLOCKS_PER_SEC; // second 초 단위로 변환

    printf("Compelete send dummy data. \n");

    printf("UDP throuput : %f byte/sec \n", total_byte / total_second);

    sendto(client_socket, "QUIT", 4, 0, (SOCKADDR *)&server_addr, sizeof(server_addr)); // UDP 전송 종료 신호

    closesocket(client_socket); // 소켓 닫기

    WSACleanup(); // 소켓 라이브러리 해제

    return 0;
}
