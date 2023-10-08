#include <iostream>
#include <vector>

#include "BlockDirective.hpp"
#include "ConfigChecker.hpp"

Checker::Checker(BlockDirective directive) {
  try {
    // 우선 첫번째 block이 http인지 확인
    if (directive.key == "server") {
      vector<Directive>::iterator iter;
      for (iter = directive.children.begin(); iter != directive.children.end();
           iter++) {
        if (iter->key == "listen") {
          /* code */
        }
        // 이런 식으로 계속 검사하기
      }
    }
  } catch (const std::exception &e) {
    std::cerr << e.what() << '\n';
  }
}

/*
우선 checker와 인스턴스화 하는 부분을 분리하고픔
1. 검사 먼저 하고 인스턴스화 하기
    - 인스턴스화 할 때 같은 내용을 또 읽어야 함
2. 인스턴스 먼저 하고 검사하기
    - 검사 중간에 문제가 생기면 모든 걸 delete 해야 함

아니면 계획했던대로 검사와 인스턴스화를 동시해 하는 방법도 존재
1. 인스턴스를 인터페이스로 먼저 생성
2. 검사하고 문제 없으면 인스턴스 채워넣기
3. 문제가 생기면 바로 throw, 인스턴스 제거
*/
