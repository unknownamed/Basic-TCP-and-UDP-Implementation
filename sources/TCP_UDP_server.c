#include <stdio.h>
#include <string.h>
#include <winsock2.h> //소켓 프로그래밍용
#include <time.h>     //throuput 측정용 ms측정을 위해 clock()함수

#define SIZE 20001//printf로 클라이언트에게 받은 내용 확인용 최대 한번 전송 2000bytes

int main()
{

    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData); // 소켓 라이브러리 초기화

    // TCP 서버
    SOCKET TCP_server_socket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP); // TCP 선택하여 소켓 생성, 접속유무 판단 소켓

    SOCKADDR_IN TCP_server_addr;
    ZeroMemory(&TCP_server_addr, sizeof(TCP_server_addr)); // 0으로 초기화

    TCP_server_addr.sin_family = AF_INET;
    TCP_server_addr.sin_addr.s_addr = htonl(ADDR_ANY);                              // 허용할 클라이언트의 주소
    TCP_server_addr.sin_port = htons(5000);                                         // 포트넘버
    bind(TCP_server_socket, (SOCKADDR *)&TCP_server_addr, sizeof(TCP_server_addr)); // 소켓에 열어놓을 주소 포트 초기화

    listen(TCP_server_socket, SOMAXCONN); // 대기줄 길이 최대

    // UDP 서버
    SOCKET UDP_server_socket = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP); // UDP 선택하여 소켓 생성, 접속유무 판단 소켓

    SOCKADDR_IN UDP_server_addr;
    ZeroMemory(&UDP_server_addr, sizeof(UDP_server_addr)); // 0으로 초기화

    UDP_server_addr.sin_family = AF_INET;
    UDP_server_addr.sin_addr.s_addr = htonl(ADDR_ANY); // 허용할 클라이언트의 주소
    UDP_server_addr.sin_port = htons(5000);            // 포트넘버

    bind(UDP_server_socket, (SOCKADDR *)&UDP_server_addr, sizeof(UDP_server_addr)); // 소켓에 열어놓을 주소 포트 초기화

    char recv_message[SIZE];
    FD_SET buffer;
    while (TRUE)
    {
        printf("Waiting for client connection.\n");

        FD_ZERO(&buffer);
        FD_SET(TCP_server_socket, &buffer); // 이벤트 감시할 소켓
        FD_SET(UDP_server_socket, &buffer);

        int buffer_in = select(0, &buffer, NULL, NULL, NULL); // TCP/UDP 소켓들의 이벤트 유무 확인, 반환 확인된 이벤트 수

        if (buffer_in == SOCKET_ERROR)
            continue; // 오류시 새로 확인

        if (FD_ISSET(TCP_server_socket, &buffer)) // 감시중인 TCP 소켓에 이벤트 발생
        {

            SOCKET TCP_client_socket = accept(TCP_server_socket, NULL, NULL); // 실제 통신할 소켓 생성

            if (TCP_client_socket == INVALID_SOCKET)
            {
                printf("TCP Client connected fail. \n");
                continue; // 연결실패시 다시 다른 클라이언트 요청을 처리
            }
            else
                printf("TCP Client connected. \n");

            clock_t start_time, end_time;
            double total_second; // 총 걸린 시간(단위 second)
            int total_byte = 0;  // 총 데이터 크기(단위 byte)

            start_time = clock(); // 데이터 받기전 측정 시작

            while (TRUE)
            {

                int recv_len = recv(TCP_client_socket, recv_message, sizeof(recv_message) - 1, 0); // 반환값 byte단위

                total_byte += recv_len;

                int disconnect = 0;
                if (recv_len == SOCKET_ERROR || recv_len == disconnect)
                {
                    printf("TCP Client disconnected. \n");

                    break;
                }

                int connect_close = 0;
                recv_message[recv_len] = '\0';
                if (strcmp(recv_message, "QUIT") == connect_close)//클라이언트의 전송 종료 메세지
                {
                    ZeroMemory(recv_message, sizeof(recv_message)); // 서버는 데이터를 수신후 삭제한다, 버퍼 비우기

                    break; // 탈출후 소켓 닫고 다음 연결시도
                }

                printf("Received from TCP client : %s \n", recv_message);
                ZeroMemory(recv_message, sizeof(recv_message)); // 서버는 데이터를 수신후 삭제한다, 버퍼 비우기
            }

            end_time = clock();

            total_second = (double)(end_time - start_time) / CLOCKS_PER_SEC; // second 초 단위로 변환

            printf("TCP throuput : %f byte/sec \n", total_byte / total_second);

            closesocket(TCP_client_socket); // 현재연결 소켓 닫기 - 다음 클라이언트 통신 확인후 다시 통신
        }

        if (FD_ISSET(UDP_server_socket, &buffer)) // 감시중인 UDP 소켓에 이벤트 발생
        {
            SOCKADDR_IN UDP_client_addr;
            int UDP_client_addr_size = sizeof(UDP_client_addr);

            clock_t start_time, end_time;
            double total_second; // 총 걸린 시간(단위 second)
            int total_byte = 0;  // 총 데이터 크기(단위 byte)
                
            while (TRUE)
            {
                int recv_len = recvfrom(UDP_server_socket, recv_message, sizeof(recv_message) - 1, 0, (SOCKADDR *)&UDP_client_addr, &UDP_client_addr_size); // 클라이언트의 요청을 버퍼에서 들고옴

                if (recv_len == SOCKET_ERROR)
                {
                    printf("UDP Socket error. \n");

                    continue; // 다른 클라이언트의 요청 처리
                }
                
                if(total_byte == 0) start_time = clock();//받은 데이터가 없다면 UDP연결 시작, 이제부터 총 소요시간 측정

                total_byte += recv_len;

                int connect_close = 0;
                recv_message[recv_len] = '\0';
                if (strcmp(recv_message, "QUIT") == connect_close)//클라이언트의 전송 종료메세지
                {
                    ZeroMemory(recv_message, sizeof(recv_message)); // 서버는 데이터를 수신후 삭제한다, 버퍼 비우기

                    break; // 탈출후 다른 요청 시도
                }

                printf("Received from UDP client : %s \n", recv_message); // 에코 서버라서 클라이언트가 보낸데이터 그대로 서버에서 확인용
                ZeroMemory(recv_message, sizeof(recv_message));           // 서버는 데이터를 수신후 삭제한다, 버퍼 비우기
            }

            end_time = clock();
            
            total_second = (double)(end_time - start_time) / CLOCKS_PER_SEC; // second 초 단위로 변환

            printf("UDP throuput : %f byte/sec \n", total_byte / total_second);
        }
    }

    // 서버는 항상 켜져있음
    return 0;
}
