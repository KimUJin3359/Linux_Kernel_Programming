# Linux_Kernel_Programming

### Kernel 프로그래밍은 version에 따라 작동여부가 달라짐
- [라즈베리파이 OS 커널 빌드 공식 사이트](https://www.raspberrypi.org/documentation/linux/kernel/building.md)
  ```
  sudo apt install git bc bison flex libssl-dev make
  // kernel source download
  cd /usr/src
  git clone --depth=1 https://github.com/raspberrypi/linux
  cd linux
  // using kernel7l because I am using raspberry ver 4
  KERNEL=kernel7l
  make bcm2711_defconfig
  // build
  // zImage : linux kernel
  // modules : device driver
  // dtbs : device driver info
  make -j4 zImage modules dtbs
  // result copy
  sudo make modules_install
  sudo cp arch/arm/boot/dts/*.dtb /boot/
  sudo cp arch/arm/boot/dts/overlays/*.dtb* /boot/overlays/
  sudo cp arch/arm/boot/dts/overlays/README /boot/overlays
  sudo cp arch/arm/boot/zImage /boot/$KERNEL.img
  ```
- 사용 커널 버전
  - 5.10.35-v7l+

---

### Device Driver
- 프로그램이 HW를 제어하기위한 SW
- Software 인터페이스를 통해 Application이 HW Spec을 이해하지 않아도 됨
- insmod : Kernel에 Device Driver Moudle을 넣어 Kernel이 해당 모듈을 관리
- rmmod : 필요 없을 때, Module을 제거(메모리상 모두 Remove)

#### Device Driver 개발 필요성
- HW 메모리 맵 Address에 직접 값 Access 가능
  - Application이 직접 HW를 제어
- HW를 사용하는 여러개의 Firmware들을 개발
- HW가 교체된다면
  - 모든 Firmware의 HW 관련 코드를 수정
  - 모든 Firmware 다운로드 후 실행
- Application과 HW 사이에 계층을 둠
  - Kernel 소스만 새로운 HW가 동작되도록 수정하여 다시 build하면, 다른 Firmware 수정 필요가 없음
  - Application은 Kernel의 API를 통해 HW 접근이 가능
- 새로운 HW 추가를 위해 커널 소스코드 수정 시 재 빌드 필요
  - Kernel만 재 빌드하면 되지만, 시간이 오래걸림
- Kernel 안에서도 2개의 Layer를 나눔
  - Kernel을 다시 빌드하지 않도록 모듈 방식 사용
  - Device Files에 API를 던짐
  - Device Driver만 재 Build하여 커널에 넣고 뻄

#### Device Driver 종류
- chrdev
  - byte 단위로 값 전달
  - 일반적인 임베디드 장치에 사용
- blkdev
  - 블록 디바이스 드라이버
  - kb 이상의 블록 단위로 값을 전달
  - disk 장치에 사용되는 디바이스 드라이버
- netdev
  - 네트워크 디바이스 드라이버
  - socket을 열고 ioctl이라는 system call로 장치를 제어

#### Device Driver 만들기
- 커널 소스코드 
  - 리눅스 커널 source code
  - 리눅스 커널 build 도구
  - 리눅스 커널 header files(커널 헤더)
  - 리눅스 커널 module build 도구(커널 헤더)
  - 커널 소스코드 / 헤더 경로
    - /usr/src
    - /lib/modules/커널버전/build
- 모듈 만들기
  - main 함수가 없음
  - 라이센스 설정을 해주어야 됨
    ```
    MODULE_LICENSE("GPL");
    ```
  - 권장
    - 커널 내부 모듈끼리 함수 중복을 피하기 위해 함수명을 '모듈명_역할' 형태로 사용
    - 모든 함수에 static
  - open, release 함수(inode, filp)
    - inode : Device File의 inode 정보를 갖음. insmod 시 inode에는 주번호/부번호 정보도 적힘
    - filp : 디바이스 파일이 app에서 어떤 형태로 읽혔는지 정보가 들어있음. ex) O_RDWR
  - char device driver 형태
    - open
    - ioctl : 제어, 설정 용도
    - read, write : 실제 값 전달 및 받기
    - close
- Makefile 만들기
  ```
  KERNEL_HEADERS = /lib/modules/$(shell uname -r)/build
  obj-m := '모듈명.o'
  all:
    make -C $(KERNEL_HEADERS) M=$(PWD) modules
  clean:
    make -C $(KERNEL_HEADERS) M=$(PWD) clean
  ```
  - C 옵션 : 해당 디렉토리로 이동해서 make를 수행
  - M=$(PWD) : 결과물이 현재 디렉토리에 생성

#### ioctl
- ioctl(fd, CMD, &args)
- 여러가지 옵션을 줄 수 있음
  - APP에서 CMD에 정수 값을 보냄
  - 드라이버에서 CMD 값에 따라 select 문으로 분기를 나눠 코드 추가 가능
  - args에 포인터를 전달하여 APP의 다량의 데이터를 driver로 전달 가능

#### ioremap
- asm/io.h에 존재
- 커널에서 HW 메모리에 접근 가능
- 물리 메모리 -> Kernel space 메모리에 mapping
- BASE를 출력하면, KERNEL Space 어디에 Mapping 했는지 알 수 있음

#### Kernel 관련 명령어
- ko 파일 정보 확인하기 : modinfo '모듈명'.ko
- 적재된 커널 module 확인 : lsmod
- 커널 로그 모니터링 : dmesg
  - w 옵션 : 실시간 

---

### Device Node
- Linux App Level에서는 Device File을 통해서 HW 컨트롤이 가능
  - Device File이 있어야 HW 컨트롤이 가능(Device File = Device Node)

#### 노드 파일
- Major Number
  - 디바이스 드라이버의 종류를 나타냄
  - 같은 기능을 하는 디바이스 드라이버가 여러개 있다면 같은 주번호를 가짐
- Minor Number
  - 같은 종류 Device에서 구분 용도
  - 개발자 마음대로 부번호 의미 부여 가능
  - blkdev에서는 파티션 번호로 사용
  - node 이름에서 숫자가 붙은 경우 부번호를 의미
- netdev는 노드를 사용하지 않음

#### 노드 만들기 명령어
- mknod
  - 일반적으로 /dev/에 생성하여 노드를 관리
  - sudo mknod /dev/'모듈명' c 100 0
    - c : chrdev
    - 100 : major number
    - 0 : minor number

---

### System call
- App 혼자서 할 수 없어 Kernel의 도움을 받아야 할 수 있는 기능
  - malloc / fork / open / socket 등
  - OS Kernel의 기능을 사용하게 해주는 API
- [System Call Number](https://filippo.io/linux-syscall-table/)

#### 동작 방식
- Interrupt 방식으로 System Call 동작
1. libc 함수 안에서 System Call Number를 레지스터(R0 ~ Rn)에 값을 넣고 SWI 수행
2. SWI로 인해 CPU 모드가 User Mode -> Kernel Mode로 전환 됨
3. ISR에서 System Call Number를 확인하여 매칭되는 함수를 호출
4. 해당 함수가 작업이 끝나면, User 모드로 돌아감
5. 스케쥴러에 의해 다시 작업을 이어서 수행

---

### Memory Map 분석
- [datasheet](https://www.raspberrypi.org/documentation/hardware/raspberrypi/datasheets.md)
- 일반적인 Memory Address는 32bit를 사용
  - Address를 갖는 메모리 한 칸당 1 Byte 공간 저장
  - 최대 4 GB 메모리의 Address 지정 가능
    - 최대 0xFFFF FFFF (32bit)
    - 1Byte * 0xFFFF FFFF = 약 4GB
  - Memory Mapped I/O를 사용하기 위해서는 여분도 필요
- 32bit system에서 4GB 이상 메모리 사용을 위한 노력
  - ARM의 "LPAE" 기능
    - Large Physical Address Extension
    - 더 큰 메모리를 사용할 수 있도록 BCM2711은 35bit 주소 체계를 사용
    - 35bit로 사용할 수 있는 최대 메모리 용량 : 약 32GB 메모리
  - LPAE Enable 시
    - Full 35bit Address Map 사용
    - 약 32GB까지 사용 가능
  - LPAE Disable 시
    - Legacy Master view Memory Map 사용
