#include <iostream>
#include <vector>

#include "BlockDirective.hpp"
#include "ConfigChecker.hpp"

RootConfig &Checker::checkDirective(BlockDirective directive) {
  try {
    std::vector<Directive>::iterator server;
    // 우선 첫번째 block이 http인지 확인
    if (directive.key == "http") {
      for (server = directive.children.begin();
           server != directive.children.end(); server++) {
        if (server->key == "server") {
          /* code */
        }
        // 이런 식으로 계속 검사하기
      }
    }
    if (directive.key == "server") {
    }
  } catch (const std::exception &e) {
    std::cerr << e.what() << '\n';
  }
}

/*
우선 checker와 인스턴스화 하는 부분을 분리하고픔
1. 검사 먼저 하고 인스턴스화 하기
    - 인스턴스화 할 때 같은 내용을 또 읽어야 함

인스턴스화 먼저 하고, 검사한 뒤에, 안에 내용 집어넣기. 만약 문제가 생긴다면
인스턴스 지워짐
*/
