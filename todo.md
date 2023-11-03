[앞으로 할 일]

1.read
  - [ ] kqueue에서 read를 해야하는지?
    - [ ] 근데 왜? 명확한 이유 알기 : read가 block 당할 상황 방지
    - [ ] 파일에 대한 read가 block 당할 수 있는지?
2. config read
  - [ ] root는 절대 경로만 허용.block (e.g. oot /www);
  - [ ] location은 경로 뒤에 무조건 / 붙는 방식 (e.g. location /test/;)
  - [ ] root 뒤에 location의 경로가 붙는 방식 (e.g. curl localhost:8080/www/test/ -v)
  - [x] alias 안 만들기
3. write
  - [ ] 받은 conf를 어떻게 적용시킬건지
    - 우선 순위: port -> server name -> server block의 순서 (default server)
  - [ ] cgi 고민해보기
4. timeout
  - [ ] read 상황 때 timeout으로 잡기
  - [ ] timeout의 시간을 몇으로 설정을 할지 (conf) : 60초
  - [ ] write 상황 때에도 timeout을 해야 하는지? 아마도?
5. 웹페이지 구상하기
  - [ ] error page, post, get, delete
