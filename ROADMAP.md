# 테트리스 C 프로젝트 — 단계별 개발 로드맵

> 플랫폼: macOS / Windows (CMake 크로스플랫폼 빌드)  
> 언어: C (C11)  
> 목표: 터미널에서 동작하는 완성형 테트리스 게임

---

## 현재 진행 상태

| 단계 | 상태 |
|------|------|
| Step 1 — 입력 시스템 수정 | ⬜ 미완료 (우선 처리 필요) |
| Step 2 — 게임 보드 그리기 | ✅ 완료 |
| Step 3 — 테트로미노 정의 | 🔧 일부 완료 (리팩토링 필요) |
| Step 4 — 블록 이동과 회전 | ⬜ 미완료 |
| Step 5 — 충돌 감지 | ⬜ 미완료 |
| Step 6 — 블록 고정과 자동 낙하 | ⬜ 미완료 |
| Step 7 — 라인 클리어와 점수 | ⬜ 미완료 |
| Step 8 — 게임 오버와 다음 블록 미리보기 | ⬜ 미완료 |

---

## CMake 빌드 환경 설정

### 개요

이 프로젝트는 CMake를 사용해 macOS와 Windows 양쪽에서 동일한 소스로 빌드한다.  
소스 파일 목록은 `CMakeLists.txt` 한 곳에서만 관리하며, 각 플랫폼의 빌드 파일(Makefile / .sln)은 `build/` 폴더 안에 자동 생성된다.

```
prj_tetris_c/
├── CMakeLists.txt   ← 소스 파일 목록 관리 (플랫폼 공통)
├── build/           ← 자동 생성, git에서 제외
│   ├── tetris       ← 실행 파일 (macOS)
│   └── tetris.exe   ← 실행 파일 (Windows)
└── tetris/          ← 소스 파일
    ├── main.c
    ├── console.c / .h
    ├── block.c / .h
    ├── input.c / .h
    ├── render.c / .h
    ├── logic.c / .h
    └── game.c / .h
```

---

### macOS 환경 설정

#### 사전 준비 (최초 1회)

CMake 설치:
```bash
brew install cmake
```

빌드 폴더 초기화 (최초 1회 또는 CMakeLists.txt 구조 변경 시):
```bash
cmake -S . -B build
```

#### 빌드 및 실행

```bash
cmake --build build   # 빌드
./build/tetris        # 실행
```

#### VSCode에서 빌드

`Cmd+Shift+B` — "Build Tetris" 태스크 실행 (configure + build 자동 수행)  
`F5` — 빌드 후 lldb 디버거로 실행

---

### Windows 환경 설정

#### 사전 준비 (최초 1회)

1. [cmake.org/download](https://cmake.org/download) 에서 CMake 설치 (또는 Visual Studio 설치 시 함께 포함됨)
2. PowerShell 또는 개발자 명령 프롬프트에서 프로젝트 루트로 이동 후 실행:

```powershell
cmake -S . -B build -G "Visual Studio 17 2022"
```

> Visual Studio 버전에 따라 Generator 이름이 다르다:
> - Visual Studio 2022: `"Visual Studio 17 2022"`
> - Visual Studio 2019: `"Visual Studio 16 2019"`

3. `build/tetris.sln` 이 생성되면 Visual Studio에서 열어서 사용

#### 빌드 및 실행

**방법 A — Visual Studio에서 직접 (권장)**  
`build/tetris.sln` 열기 → `Ctrl+Shift+B` (빌드) → `F5` (실행)

**방법 B — 명령줄**
```powershell
cmake --build build --config Debug
```

#### Visual Studio에서 CMake 직접 열기 (VS 2019 이상)

`파일 > 열기 > CMake...` → 프로젝트 루트의 `CMakeLists.txt` 선택  
이 방식에서는 `.sln` 없이 VS가 CMake를 직접 읽고, `CMakeLists.txt` 수정 시 자동으로 프로젝트를 갱신한다.

---

### 소스 파일 추가 방법

새 `.c` / `.h` 파일을 추가할 때는 `CMakeLists.txt`의 `add_executable` 목록에 파일명을 추가한다.

```cmake
add_executable(tetris
    tetris/main.c
    tetris/console.c
    tetris/block.c
    tetris/input.c
    tetris/render.c
    tetris/logic.c
    tetris/game.c
)
```

이후:
- **macOS**: 다음 `cmake --build build` 실행 시 자동 반영
- **Windows (명령줄)**: `cmake -S . -B build -G "Visual Studio 17 2022"` 재실행 후 빌드
- **Windows (VS에서 CMake 직접 열기)**: 저장 시 VS가 자동으로 갱신

---

---

## Step 1 — 입력 시스템 수정 ⬜

### 목표
macOS에서 게임 루프가 멈추는 문제를 해결하고, 색상 출력 버그(`TextColor`)를 수정한다.

### 배경 지식

**non-blocking 입력**  
현재 `GetKey()`는 `getchar()`를 사용해 입력이 올 때까지 루프 전체를 멈춘다.  
`select()` 시스템 콜을 사용하면 입력이 없을 때 즉시 `-1`을 리턴해 루프가 계속 돌 수 있다.

```c
// select()를 이용한 non-blocking 입력 체크 (macOS/Linux)
struct timeval tv = { 0, 0 };  // 대기 시간 0 → 즉시 리턴
fd_set fds;
FD_ZERO(&fds);
FD_SET(STDIN_FILENO, &fds);
if (select(1, &fds, NULL, NULL, &tv) <= 0) return -1;  // 입력 없음
```

### 작업 내용
- [x] **`console.c` — macOS `TextColor()` ANSI 매핑 테이블 추가 및 적용** *(버그 수정)*
- [x] `input.c` — macOS `GetKey()`를 `select()` 기반 non-blocking으로 교체
- [ ] `game.c` — ESC 또는 Q 키 입력 시 게임 루프 종료 및 터미널 상태 복구

### 완료 기준
- 키 입력 없이도 게임 루프가 멈추지 않고 계속 돈다.
- 색상이 의도한 대로 터미널에 출력된다.
- ESC 또는 Q 키로 게임을 종료할 수 있다.

---

## Step 2 — 게임 보드 그리기 ✅

### 목표
테트리스 게임판(보드)을 화면에 출력하고, 2D 배열로 내부 상태를 관리한다.

### 배경 지식
- 표준 테트리스 보드 크기: 가로 10칸, 세로 20칸
- 터미널에서 블록 한 칸은 전각 문자(`■`, `□`) 1개 = 실제 터미널 폭 2칸
- 보드 상태를 `int board[ROWS][COLS]` 2D 배열로 관리 (0 = 빈칸, 1 이상 = 블록 있음)

### 구현 현황
- [x] `game.h` — 보드 크기 상수 정의 (`BOARD_ROWS 20`, `BOARD_COLS 10`)
- [x] `game.h` — 보드 원점 상수 정의 (`BOARD_ORIGIN_X 2`, `BOARD_ORIGIN_Y 1`)
- [x] `game.c` — `int board[BOARD_ROWS][BOARD_COLS]` 전역 배열 선언
- [x] `render.c` — `DrawBorder()`: 보드 테두리(벽, 바닥) 출력
- [x] `render.c` — `DrawBoard()`: 보드 배열 상태를 화면에 렌더링
- [x] `game.c` — `RunGame()`에서 시작 시 보드 출력

---

## Step 3 — 테트로미노 정의 🔧

### 목표
7가지 테트로미노(블록) 형태를 데이터로 정의하고 화면에 출력한다.

### 배경 지식

**테트로미노 7종과 4×4 격자 표현**  
각 블록은 4×4 격자 안에서 표현한다. 회전 시에도 격자를 벗어나지 않아 안전하다.

```
I 블록 (SKYBLUE):     O 블록 (YELLOW):     T 블록 (PURPLE):
0 0 0 0               0 0 0 0              0 0 0 0
2 2 2 2               0 2 2 0              0 2 2 2
0 0 0 0               0 2 2 0              0 0 2 0
0 0 0 0               0 0 0 0              0 0 0 0

J 블록 (DARKBLUE):    L 블록 (DARKYELLOW): S 블록 (GREEN):     Z 블록 (RED):
0 0 0 0               0 0 0 0              0 0 0 0             0 0 0 0
2 2 2 0               0 2 2 2              0 0 2 2             0 2 2 0
0 0 2 0               0 2 0 0              0 2 2 0             0 0 2 2
0 0 0 0               0 0 0 0              0 0 0 0             0 0 0 0
```

### 구현 현황
- [x] `block.h` — `Tetromino` 구조체 정의: `shape[4][4]`, `color`
- [x] `block.h` — `ActiveBlock` 구조체 정의: `tetromino`, `x`, `y`
- [x] `block.h` — 7종 블록 초기 형태 및 색상 데이터 정의
- [x] `render.c` — `DrawTetromino()`: 블록을 화면에 출력
- [x] `render.c` — `EraseTetromino()`: 블록을 지우기

### 남은 작업 (리팩토링 포함)
- [x] **`block.c` 파일 생성 — 블록 변수 정의를 헤더에서 분리** *(버그 수정)*
- [x] **`block.h` — 블록 변수를 `extern` 선언으로 변경** *(버그 수정)*
- [x] **`CMakeLists.txt` — `tetris/block.c` 추가** *(버그 수정)*
- [x] **`render.c` — `DrawTetromino()`, `EraseTetromino()` 좌표 스케일 버그 수정 (`j * 2`)** *(버그 수정)*
- [x] `logic.c` — `SpawnTetromino()`: 7종 중 랜덤으로 선택해 보드 상단 중앙에 생성

### 완료 기준
- 실행하면 보드 상단에 랜덤 블록이 하나 표시된다.
- 블록이 가로로 겹치지 않고 올바른 크기로 출력된다.
- 블록이 의도한 색상으로 출력된다.
- `block.h`를 여러 파일에서 include해도 링크 에러가 없다.

---

## Step 4 — 블록 이동과 회전 ⬜

### 목표
방향키로 블록을 이동하고, 위 방향키로 블록을 회전시킨다.

### 배경 지식

**이동**  
`ActiveBlock`의 `x`, `y` 좌표를 변경하기 전에 `EraseTetromino()`로 지우고, 변경 후 `DrawTetromino()`로 다시 그린다. (Step 5에서 충돌 검사를 추가하기 전까지는 경계를 넘을 수 있다.)

**회전 (4×4 행렬 90도 시계 방향)**  
4×4 행렬을 90도 시계 방향으로 회전하는 공식:
```c
new_shape[j][3 - i] = old_shape[i][j];   // i: 행, j: 열
```

**하드 드롭 (스페이스 키)**  
스페이스를 누르면 블록이 충돌 없이 내려갈 수 있는 가장 아래 위치까지 즉시 이동한다.  
(Step 5 충돌 감지 구현 후에 완성 가능)

### 작업 내용
- [ ] `logic.c` — `MoveTetromino(int dx, int dy)`: x, y 좌표 변경
- [ ] `logic.c` — `RotateTetromino()`: 4×4 행렬 90도 시계 방향 회전
- [ ] `logic.c` — `HardDrop()`: 스페이스 키로 즉시 최하단 낙하 (Step 5 이후 완성)
- [ ] `game.c` — 키 입력과 각 함수 연결 (←→↓: 이동, ↑: 회전, Space: 하드 드롭)
- [ ] `game.c` — 이동/회전 시 Erase → 변경 → Draw 순서 보장

### 완료 기준
- 좌/우/아래 방향키로 블록이 이동한다.
- 위 방향키로 블록이 90도 회전한다.
- 이동 잔상 없이 깔끔하게 갱신된다.

---

## Step 5 — 충돌 감지 ⬜

### 목표
블록이 벽, 바닥, 이미 고정된 블록과 겹치지 않도록 막는다.

### 배경 지식
이동/회전을 적용하기 전에 목적지 위치가 유효한지 미리 검사한다.  
검사 실패 시 이동/회전을 취소하고 좌표를 원복한다.

검사 대상:
- 블록의 각 셀 좌표가 보드 범위(0~BOARD_COLS-1, 0~BOARD_ROWS-1) 안에 있는가
- 해당 보드 배열 칸이 이미 채워져 있는가 (`board[row][col] != 0`)

```c
int IsValidPosition(ActiveBlock *ab) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (ab->tetromino.shape[i][j] == 0) continue;
            int row = ab->y + i;
            int col = ab->x + j;
            if (col < 0 || col >= BOARD_COLS) return 0;  // 좌우 벽
            if (row >= BOARD_ROWS) return 0;              // 바닥
            if (board[row][col] != 0) return 0;           // 고정 블록
        }
    }
    return 1;
}
```

### 작업 내용
- [ ] `logic.c` — `IsValidPosition(ActiveBlock *ab)`: 위치 유효성 검사
- [ ] `logic.c` — `MoveTetromino()` 에 검사 추가 → 실패 시 좌표 원복
- [ ] `logic.c` — `RotateTetromino()` 에 검사 추가 → 실패 시 회전 취소
- [ ] `logic.c` — `HardDrop()` 완성 (IsValidPosition으로 가장 아래 위치 탐색)

### 완료 기준
- 블록이 보드 밖으로 나가지 않는다.
- 고정된 블록 위로 겹쳐지지 않는다.
- 회전 시 벽을 뚫지 않는다.
- 스페이스 키로 즉시 최하단에 내려놓을 수 있다.

---

## Step 6 — 블록 고정과 자동 낙하 (중력) ⬜

### 목표
블록이 바닥 또는 다른 블록에 닿으면 고정되고, 시간이 지나면 자동으로 아래로 떨어진다.

### 배경 지식

**블록 고정**  
아래 이동이 불가능한 경우, 현재 블록의 각 셀 좌표를 `board` 배열에 기록한다.  
이 시점에 블록의 색상 정보도 함께 저장하면 나중에 색상을 유지한 채 보드를 렌더링할 수 있다.

```c
// board에 색상 인덱스를 저장하는 방식
board[row][col] = (int)activeBlock.tetromino.color + 1;  // 0은 빈칸이므로 +1
```

**타이머 (자동 낙하)**  
게임 루프 안에서 경과 시간을 측정해 일정 간격마다 블록을 한 칸 아래로 이동시킨다.  
`gettimeofday()`로 현재 시각을 가져와 마지막 낙하 시각과 비교한다.

```c
#include <sys/time.h>

struct timeval now;
gettimeofday(&now, NULL);
long elapsed_ms = (now.tv_sec - last.tv_sec) * 1000
                + (now.tv_usec - last.tv_usec) / 1000;
if (elapsed_ms >= drop_interval_ms) { /* 한 칸 낙하 */ }
```

### 작업 내용
- [ ] `logic.c` — `LockTetromino()`: 현재 블록 위치를 `board[][]`에 기록
- [ ] `logic.c` — `SpawnTetromino()`: 새 블록을 상단 중앙에 생성
- [ ] `game.c` — 게임 루프에 `gettimeofday()` 기반 타이머 추가
- [ ] `game.c` — 일정 간격마다 자동 하강 (초기값 500ms)
- [ ] `game.c` — 아래 이동 불가 시 LockTetromino → SpawnTetromino 흐름 연결

### 완료 기준
- 블록이 자동으로 아래로 내려온다.
- 바닥/블록에 닿으면 고정되고 새 블록이 나타난다.
- 아래 방향키로 빠르게 내리기가 된다.
- 색상 정보가 보드에 저장되어 고정 후에도 색이 유지된다.

---

## Step 7 — 라인 클리어와 점수 ⬜

### 목표
가로줄이 꽉 차면 해당 줄을 지우고 위 블록을 내리며, 점수를 부여한다.

### 배경 지식

**라인 클리어 절차**  
1. 블록 고정 직후, 모든 행을 아래에서 위 방향으로 검사
2. 한 행의 모든 칸(`BOARD_COLS`개)이 채워졌으면 해당 행을 삭제
3. 삭제된 행 위의 모든 행을 한 칸씩 아래로 복사

**점수 계산 (테트리스 공식)**

| 한 번에 지운 줄 수 | 점수 |
|------|------|
| 1줄 | 100 × 레벨 |
| 2줄 | 300 × 레벨 |
| 3줄 | 500 × 레벨 |
| 4줄 (테트리스) | 800 × 레벨 |

**레벨 업**  
총 클리어한 줄 수가 `레벨 × 10`에 도달하면 레벨이 1 오르고, 낙하 속도가 빨라진다.

```
레벨별 낙하 간격 예시:
레벨 1: 500ms, 레벨 2: 450ms, ..., 레벨 10: 100ms
```

### 작업 내용
- [ ] `logic.c` — `CheckAndClearLines()`: 꽉 찬 줄 탐색, 삭제, 위 블록 내리기
- [ ] `game.c` — `score`, `level`, `lines_cleared` 전역 변수 추가
- [ ] `render.c` — `DrawScore()`: 점수, 레벨, 클리어 줄 수를 보드 옆에 출력
- [ ] `game.c` — 레벨에 따라 낙하 간격(`drop_interval_ms`) 조정

### 완료 기준
- 줄이 꽉 차면 사라지고 위 블록이 내려온다.
- 점수와 레벨이 화면에 표시되고 갱신된다.
- 레벨이 오를수록 블록이 빠르게 내려온다.

---

## Step 8 — 게임 오버와 다음 블록 미리보기 ⬜

### 목표
게임 오버 조건을 처리하고, 다음에 나올 블록을 미리 보여준다.

### 배경 지식

**게임 오버 조건**  
새 블록이 스폰되는 위치(보드 상단 중앙)에 이미 고정된 블록이 있으면 게임 종료.  
즉, `SpawnTetromino()` 직후 `IsValidPosition()`이 실패하면 게임 오버다.

**다음 블록 미리보기**  
`SpawnTetromino()` 호출 시 현재 블록과 다음 블록을 동시에 결정해 저장한다.  
다음 블록은 보드 오른쪽 옆의 별도 영역에 출력한다.

**게임 초기화**  
재시작 시 다음 상태를 초기화해야 한다:
- `board[][]` → 전부 0
- `score`, `level`, `lines_cleared` → 0
- 현재 블록, 다음 블록 → 새로 스폰
- 낙하 간격 → 초기값(500ms)
- 타이머 → 현재 시각으로 리셋

### 작업 내용
- [ ] `logic.c` — `SpawnTetromino()`에서 스폰 실패 시 게임 오버 상태 진입
- [ ] `render.c` — `DrawGameOver()`: 게임 오버 메시지 및 재시작 안내 출력
- [ ] `game.c` — `InitGame()`: 게임 전체 상태 초기화 함수
- [ ] `game.c` — R 키로 `InitGame()` 호출해 재시작
- [ ] `game.c` — `next_block` 변수 추가 (`SpawnTetromino()` 시 미리 결정)
- [ ] `render.c` — `DrawNextBlock()`: 보드 오른쪽 영역에 다음 블록 미리보기 출력

### 완료 기준
- 블록이 최상단을 넘으면 게임 오버 메시지가 표시된다.
- R 키로 게임이 완전히 초기화되어 재시작된다.
- 보드 옆에 다음 블록이 미리 표시된다.

---

## 참고 — 파일 역할 분담

| 파일 | 담당 |
|------|------|
| `console.c / .h` | 터미널 유틸 — 커서 이동, 색상, 커서 표시/숨김 |
| `block.c / .h` | 테트로미노 데이터 — 7종 블록 형태·색상 정의 |
| `input.c / .h` | 입력 처리 — `GetKey()`, 키 상수 (`LEFT`, `RIGHT`, `UP`, `DOWN`, ...) |
| `render.c / .h` | 렌더링 — `DrawBorder`, `DrawBoard`, `DrawTetromino`, `EraseTetromino` |
| `logic.c / .h` | 게임 로직 — `SpawnTetromino`, 이동·회전·충돌·고정·라인 클리어 (Step 4~7에서 추가 예정) |
| `game.c / .h` | 게임 상태 — `board[][]`, `RunGame`, `InitGame` (Step 8에서 추가 예정) |
| `main.c` | 진입점만 — `RunGame()` 호출 |

## 참고 — 빌드 방법

빌드 방법은 상단 **CMake 빌드 환경 설정** 섹션을 참고한다.
