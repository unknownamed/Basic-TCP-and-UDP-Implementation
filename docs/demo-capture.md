# TCP / UDP 실행 GIF

원본 C 파일 6개를 Windows에서 Zig 0.15.2의 C 컴파일러와 Winsock 라이브러리로 빌드했습니다. 실제 프로세스의 콘솔 출력을 1280×720 GIF로 정리했습니다.

## 빌드 예시

`sources/`에서 실행합니다.

```powershell
zig cc -O2 -Wall TCP_echo_server.c -o TCP_echo_server.exe -lws2_32
zig cc -O2 -Wall TCP_client.c -o TCP_client.exe -lws2_32
zig cc -O2 -Wall UDP_echo_server.c -o UDP_echo_server.exe -lws2_32
zig cc -O2 -Wall UDP_client.c -o UDP_client.exe -lws2_32
zig cc -O2 -Wall TCP_UDP_server.c -o TCP_UDP_server.exe -lws2_32
zig cc -O2 -Wall TCP_UDP_client.c -o TCP_UDP_client.exe -lws2_32
```

## 캡처한 흐름

- TCP: 서버 시작, 클라이언트 연결, 두 메시지 전송과 에코 응답, quit 종료
- UDP: 두 데이터그램의 송수신과 quit 종료
- 전송 실험: TCP 500 bytes/sec, UDP 1,000 bytes/sec를 선택해 각각 5초간 전송

연결 대상은 원본 코드의 `127.0.0.1:5000`입니다. 속도 실험의 출력값은 해당 로컬 실행 조건에서 얻은 단일 실행 결과입니다. 기존 실험 보고서는 README 아래에 그대로 유지했습니다.
