[앞으로 할 일]

1.read  
    - [x] ~~kqueue에서 read를 해야하는지?~~  
      - [x] ~~근데 왜? 명확한 이유 알기 : read가 block 당할 상황 방지~~  
      - [x] ~~파일에 대한 read가 block 당할 수 있는지?~~  
    - [x] 모든 read와 write는 io event를 통해서 진행...  

1. config read
     - [x] root는 절대 경로만 허용.block (e.g. root /www);
       - root /www/; 형식같이 적는 방법도 허용 (경로를 명확하게 구분하고 디렉토리임을 나타내는 방식)
       - 결론: 둘 다 허용 => 뒤쪽 `/`를 빼서 저장
     - [x] location은 경로 뒤에 무조건 / 붙는 방식 (e.g. location /test/;)
       - location /test; -> 해당 경로에 정확하게 일치가 되어야 허용 (/test/123 같은 형식은 불가능)
       - location /test/; -> 모든 경로에 대한 설정을 허용 (/test/123 같은 경우 가능)
       - 결론: 앞에는 무조건 `/` 포함, 뒤에는 둘 다 허용 => 앞에 `/`가 없으면 error, 뒤에는 그대로 저장
     - [x] root 뒤에 location의 경로가 붙는 방식 (e.g. curl localhost:8080/www/test/ -v)
     - [x] alias 안 만들기
2. write
     - [x] 받은 conf를 어떻게 적용시킬건지
       - 우선 순위: port -> server name -> server block의 순서 (default server)
         - startline and host는 필수
     - [ ] cgi 고민해보기
3. timeout
     - [ ] read 상황 때 timeout으로 잡기
     - [ ] write 상황 때에도 timeout을 해야 하는지? 아마도?
     - [ ] timeout의 시간을 몇으로 설정을 할지 (conf) : 60초
4. 웹페이지 구상하기
     - [ ] error page
       - [ ] `error_page <http status code> <uri>`
     - [ ] post
     - [ ] get
       - [ ] autoindex
     - [ ] delete
5. cgi
6. redirection
