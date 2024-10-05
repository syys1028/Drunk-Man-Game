# Drunk-Man-Game
💡 [ Protfolio Project 001] 술 취한 사람 길찾기 게임 프로젝트

## 📌 프로젝트 소개
술취한 사람 길찾기는 C 언어로 구현한 텍스트 기반 게임입니다. 사용자는 술 취한 사람을 조작하여 여러 장애물을 피하고 집에 도착하는 것을 목표로 합니다. 술집에서 시작해 무작위로 설정된 맵에서 이동하며, 경찰과 저격수 등의 방해를 피해야 합니다.  
![image](https://github.com/user-attachments/assets/eadd34d0-d70e-4680-89e8-408c335103d1)
![image](https://github.com/user-attachments/assets/c16c1849-1929-49e6-bfed-8b3cc7889c7e)

## 📌 주요 기능
### - 맵 구조: 
  맵은 10x10 크기의 격자로 구성되어 있으며, 벽과 길로 이루어져 있습니다. 사람은 벽을 제외한 경로를 통해 이동합니다.
### - 사분면 이동: 
  맵은 1~4사분면으로 나누어져 있으며, 술집은 1사분면에, 집은 4사분면에 랜덤으로 배치됩니다. 술 취한 사람은 술집에서 출발하여 집까지 무작위로 이동합니다.
### - 자동 및 수동 이동 모드: 
  프로그램 실행 시 자동 이동과 수동 이동 횟수를 입력받습니다. 자동 이동 모드에서는 랜덤한 방향으로 자동 이동하며, 수동 이동 모드에서는 키보드를 조작하여 직접 상하좌우를 이동할 수 있습니다.
### - 장애물: 
  술 취한 사람의 이동을 방해하는 경찰과 저격수가 등장하며, 이들은 술 취한 사람이 일정 조건을 만족하지 못할 때 맵에 배치됩니다.
### - 승리 조건: 
  술 취한 사람이 경찰과 저격수를 피하고 집에 도착하면 게임에서 승리합니다.
### - 게임 규칙:
  - 술 취한 사람의 출발: 게임은 술집에서 술 취한 사람이 술집에서 출발하면서 시작됩니다. 시작은 자동 이동 모드로 진행되며, 일정 횟수 동안 무작위로 이동합니다.  
  - 경찰 등장: 만약 술 취한 사람이 지정된 자동 이동 횟수 안에 다음 사분면으로 넘어가지 못하면 경찰이 등장합니다. 이때부터 수동 이동 모드로 전환되며, 경찰의 상하좌우 1칸 내에 위치하면 발각됩니다.  
  - 저격수 등장: 경찰 등장 후에도 술 취한 사람이 집이 있는 사분면에 도착하지 못하면 저격수가 등장해 술 취한 사람을 공격하려 합니다.  
  - 목표: 술 취한 사람은 경찰과 저격수를 피해 안전하게 집에 도착해야 합니다. 경찰이나 저격수에 발각되면 게임이 종료됩니다.  
   
  
## 📌 구현 상세
### - 맵 구조:
  map[10][10] 배열을 사용해 맵을 구성합니다.  
  벽은 ■, 길은 □, 술집 B, 집은 H, 술 취한 사람은 D로 표시되며, 경찰은 P1 및 P2, 저격수는 S로 나타납니다.
### - 자동 이동 모드:
  초기 자동 이동 모드에서는 술 취한 사람이 무작위로 이동합니다. 자동 이동 횟수를 소모하면 해당 분면에 경찰이 등장합니다. 경찰에게 발각되면 수동 이동 모드로 전환됩니다.
### - 수동 이동 모드:
  수동 이동 모드로 전환되면 사용자는 키보드 화살표를 사용해 술 취한 사람을 상하좌우로 직접 이동시킬 수 있습니다. 이동 횟수가 끝나기 전에 경찰을 피해 분면을 이동해야 합니다.
### - 장애물:
  술 취한 사람이 이동 도중 경찰이나 저격수의 시야에 들어가면 게임이 종료됩니다.  경찰은 술 취한 사람과 같은 분면에서 무작위로 움직이며, 저격수는 고정된 위치에서 술 취한 사람을 저격하려 시도합니다. 저격수는 상하좌우로 벽에 닿기 전까지의 시야를 갖습니다. 
  
## 📌 개발 환경
  C 언어  
  Visual Studio 2019
