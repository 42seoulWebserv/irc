# webserv

- [webserv](#webserv)

## Convention

### header 선언 순서

```md
c++ header

c header

우리가 만든 헤더
```

### class 선언 순서

public -> protected -> private

```c++
class name {
public:
  name(void);
  name(int idx);
  name(const name &src);
  name &operator=(const name &rhs);
  ~name(void);

  void anything(void);

  int idx;
}
```

### naming

변수 이름은 무조건 snake_case

```md
변수: 변수_이름
멤버 변수: 변수_이름_

const kConstVar;
```

```md
ThisIsFunction();
get_var_name_();
set_var_name_();
```

```md
enum class UrlTableError {
  kOk = 0,
  kOutOfMemory,
  kMalformedInput,
};
```

### rules

```md
- return은 괄호 없이
- space 2칸, 80 column 최대
```
