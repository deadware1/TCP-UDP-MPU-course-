#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>

void main(int argc, char **argv) {
  int sockfd;
  int n;
  //массивы для отсылаемой и принятой строки 
  char sendline[1000], recvline[1000];
  //структура для размещения адреса сервера
  struct sockaddr_in servaddr;
  
  if (argc != 2) 
  {
    printf("Неверное количество аргументов\nВведите IP-адрес\n");
    exit(1);
  }
  //создаем TCP-сокет
  if ((sockfd = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
    perror(NULL);
    exit(1);
  }
  //перед заполнением обнуляем всю структуру
  memset(&servaddr, 0, sizeof(servaddr));
  servaddr.sin_family = AF_INET; //семейство
  servaddr.sin_port = htons(51000); //хоста
  //преобразует адрес в нужную форму и сохраняет в поле структуры
  if (inet_aton(argv[1], &servaddr.sin_addr) == 0) {
    printf("Неверный IP-адрес\n");
    close(sockfd);
    exit(1);
  }
  //устанавливаем соединение через созданный сокет с сокетом сервера, адрес которого в структуре
  if (connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
    perror(NULL);
    close(sockfd);
    exit(1);
  }
  
   //порождаем новый процесс
  pid_t pid = fork();
  if (pid == 0) //дочерний процесс - читает и выводит присылаемые сообщения 
    for (;;) {
      //обнулить строку перед чтением
      memset(recvline, 0, 1000);
      //принимаем данные
      while ((n = read(sockfd, recvline, 999)) > 0) {
        printf("\nСобеседник: %s\n", recvline);
      }
      //если при чтении возникла ошибка - завершаем работу
      if (n < 0) {
        perror(NULL);
        close(sockfd);
        exit(1);
      }
    }  
  else  // родительский процесс - отправляет сообщения
    for (;;) {
      printf("Вы:\n");
      fflush(stdin);
      //обнуляем перед записью
      memset(sendline, 0, 1000);
      //заносим в sendline сообщение
      fgets(sendline, 1000, stdin);
      //отправляем данные
      while ((n = write(sockfd, sendline, strlen(sendline) + 1)) < 0) {       
        perror(NULL);
        close(sockfd);
        exit(1);
      }        
  } 
}
