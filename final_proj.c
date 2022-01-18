#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

void main_menu();
void login();
void sign_up();
void user_menu(int);
void MoviesList(int);
void basket(int, int);
void TicketsList(int);
void PaymentsList(int);
void AddMovies(int);
void EditeMovie(int);
void DeleteMovie(int);
int over_date(int, int, int, int, int);

int main() {
  main_menu();

  return 0;
}

void main_menu() {
  int select;
  printf("--------------------------------------------------------------------\n");
  printf("# Menu\n");
  printf("1. Login\n");
  printf("2. Sign Up\n");
  printf("-> Enter the command : ");
  scanf("%d", &select);

  if (select==1)
    login();
  if (select==2)
    sign_up();
  else{
    printf("! Your request was not accepted.\n");
    main_menu();
  }
}

void login(){
  int id;
  char username[21];
  char password[21];
  char name[21];
  char enteredUser[21];
  char enterdPass[21];
  int flag=0;
  FILE *users = fopen("users.txt", "r");
  printf("--------------------------------------------------------------------\n");
  printf("# Login\n");
  printf("-> Username : ");
  scanf("%s", enteredUser);
  printf("-> Password : ");
  scanf("%s", enterdPass);

  strlwr(enteredUser);
  while(!feof(users)){
    fscanf(users, "%d %s %[^,],%s %*d %*s", &id, username, name, password);
    if(strcmp(enteredUser, username)==0 && strcmp(enterdPass, password)==0)
    {
      flag=1;
      break;
    }
  }
  fclose(users);
  if(flag==1){
    printf("* Wellcome %s\n", name);
    user_menu(id);
  }
  else{
  printf("Username or password not found!\n");
  login();
}
}
int is_exist_uername(char* word){
  FILE *users = fopen("users.txt", "r");
  char usernames[21];
  int flag=1;
  while(!feof(users)){
    fscanf(users,"%*d %s %*[^,],%*s %*d %*s", usernames);
    if(strcmp(usernames, word)==0){
      printf("-> This username is exist, Please try again!\n");
      flag=0;
      break;
    }
  }
  return flag;
}

void sign_up(){

  FILE *users = fopen("users.txt", "a+");
  int id=1;
  char name[21];
  char username[21];
  char password[21];
  srand(time(NULL));
  int cash= rand();
  char c;
  while((c=getc(users)) != EOF)
    if(c=='\n')
      id++;

  printf("--------------------------------------------------------------------\n");
  printf("# Sign Up\n");

  do{
    printf("-> Enter your username : ");
    scanf("%s", username);
  }while(is_exist_uername(username)==0);

  printf("-> Enter your name : ");
  scanf("%*c%[^\n]", name);

  printf("-> Enter password : ");
  scanf("%s", password);

  strlwr(username);
  fprintf(users, "%04d %-20s %-20s,%-20s %05d %s\n", id, username, name, password, cash, "NOT-ADMIN");
  fclose(users);
  printf("Sign Up is successful!\n");
  user_menu(id);
}

void user_menu(int userID){
  int select;
  char *type[10];
  int cash;
  FILE *users = fopen("users.txt", "r");
  for (int i = 0; i < userID; i++)
    fscanf(users,"%*d %*s %*[^,],%*s %d %s", &cash, type);
  fclose(users);
  printf("--------------------------------------------------------------------\n");
  printf("# Menu\t\t\t\t\t\t\tCash: %5d$\n", cash);
  printf("0.Exit\n");
  printf("1.List Movies\n");
  printf("2.List My Tickets\n");
  if(strcmp(type, "ADMIN")==0){
    printf("3.List All Payments\n");
    printf("4.Add Movie\n");
  }
  printf("-> Enter the command : ");
  scanf("%d", &select);

  if(select==1)
    MoviesList(userID);
  if(select==2)
    TicketsList(userID);
  if(select==3 && strcmp(type, "ADMIN")==0)
    PaymentsList(userID);
  if(select==4 && strcmp(type, "ADMIN")==0)
    AddMovies(userID);
  if(select==0)
    exit(0);
  else{
    printf("! Your request was not accepted.\n");
    user_menu(userID);
  }
}

int over_date(int y, int mon, int d, int h, int min){
  int flag = 0;
  struct tm movie_time;
  movie_time.tm_year=y-1900;
  movie_time.tm_mon=mon-1;
  movie_time.tm_mday=d;
  movie_time.tm_hour=h;
  movie_time.tm_min=min;
  time_t mt = mktime(&movie_time);
  time_t t = time(NULL);

  if(difftime(t, mt)>0)
    flag=1;

  return flag;
}

void MoviesList(int userID){
  FILE *movies = fopen("Movies.txt", "r");
  char *type[10];
  int cash;
  int selectItem;
  FILE *users = fopen("users.txt", "r");
  for (int i = 0; i < userID; i++)
    fscanf(users,"%*d %*s %*[^,],%*s %d %s", &cash, type);
  fclose(users);
  printf("--------------------------------------------------------------------\n");
  printf("# List of movies :\n\n");
  if(movies == NULL){
    fclose(movies);
    printf("*** N0 movies have been added yet ***\n\n");
    printf("0.Back : \n");
    printf("-> Enter the command : ");
    scanf("%d", &selectItem);
    if(selectItem == 0)
      user_menu(userID);
  }else{
    char name[21][1000];
    int *duration = malloc(10000*sizeof(int));
    int *capacity = malloc(10000*sizeof(int));
    int *price = malloc(10000*sizeof(int));
    int *year = malloc(10000*sizeof(int));
    int *month = malloc(10000*sizeof(int));
    int *day = malloc(10000*sizeof(int));
    int *hour = malloc(10000*sizeof(int));
    int *minute = malloc(10000*sizeof(int));
    int select;
    int i=0, j=0;
    printf("Index\tName\t\t     Duration\t\t Capacity\t\t Price\t\t Date\t\t Time\n");
    while(!feof(movies)){
      fscanf(movies,"%*d %[^,],%d %d %d %d-%d-%d %d:%d",
      name[i], &duration[i], &capacity[i], &price[i], &year[i], &month[i], &day[i], &hour[i], &minute[i]);
      i++;
    }
    for(; j<i-1; j++){
      printf("%04d \t%-20s %d\t\t %d\t\t\t %d\t\t %4d-%02d-%02d\t %02d:%02d\n",
      (j+1), name[j], duration[j], capacity[j], price[j], year[j], month[j], day[j], hour[j], minute[j]);
    }
    fclose(movies);
    printf("\n0.Back\n");
    printf("1.Buy a ticket\n");
    if(strcmp(type, "ADMIN")==0){
      printf("2.Edete a movie\n");
      printf("3.Delet a movie\n");
    }
    printf("-> Enter the command : ");
    scanf("%d", &selectItem);
    if(selectItem == 0)
      user_menu(userID);
    if(selectItem == 1){
      printf("-> Enter the index of movie you want to buy : ");
      scanf("%d", &select);
      if(select<=i && select>0){
        while (1) {
          if(over_date(year[select-1], month[select-1], day[select-1], hour[select-1], minute[select-1])){
            printf("! The release date of the movie is over.\n");
          }
          else if(capacity[select-1]<1){
            printf("! The hall is full.\n");
          }
          else if(price[select-1]>cash){
            printf("! You have not enogh mony.\n");
          }
          else{
            break;
          }
          printf("-> Enter another index of movie (0 For back): ");
          scanf("%d", &select);
          if(select == 0)
            MoviesList(userID);
        }
        free(duration);
        free(capacity);
        free(price);
        free(year);
        free(month);
        free(day);
        free(hour);
        free(minute);
        fclose(movies);
        basket(userID, select);
      }else{
        printf("! Your request was not accepted.\n");
        user_menu(userID);
      }
    }
    if(selectItem == 2 && strcmp(type, "ADMIN")==0)
      EditeMovie(userID);
    if(selectItem == 3 && strcmp(type, "ADMIN")==0)
      DeleteMovie(userID);
    else{
      printf("! Your request was not accepted.\n");
      user_menu(userID);
    }
  }
}

void basket(int userID, int movieIndex){
  FILE *users = fopen("users.txt", "r");
  FILE *movies = fopen("Movies.txt", "r");
  char nameOfMovie[21][1000];
  int *duration = malloc(10000*sizeof(int));
  int *capacity = malloc(10000*sizeof(int));
  int *price = malloc(10000*sizeof(int));
  int *year = malloc(10000*sizeof(int));
  int *month = malloc(10000*sizeof(int));
  int *day = malloc(10000*sizeof(int));
  int *hour = malloc(10000*sizeof(int));
  int *minute = malloc(10000*sizeof(int));
  int select;
  int i=0, j=0;
  int k=0, m=0;
  while(!feof(movies)){
    fscanf(movies,"%*d %[^,],%d %d %d %d-%d-%d %d:%d",
    nameOfMovie[i], &duration[i], &capacity[i], &price[i], &year[i], &month[i], &day[i], &hour[i], &minute[i]);
    i++;
  }
  printf("--------------------------------------------------------------------\n");
  printf("# Basket :\n");
  printf("-> Are you sure buy this movie (pay %d $)?\n", price[movieIndex-1]);
  printf("1.YES\n");
  printf("2.NO\n");
  printf("-> Enter the command : ");
  scanf("%d", &select);

  if(select==1){
    int id[1000];
    char nameOfUser[21][10000];
    char username[21][10000];
    char password[21][10000];
    int cash[1000];
    char type[10][10000];

    while(!feof(users)){
      fscanf(users,"%d %s %[^,],%s %d %s", &id[k], username[k], nameOfUser[k], password[k], &cash[k], type[k]);
      k++;
    }
    cash[userID-1]-=price[movieIndex-1];
    FILE *users = fopen("users.txt", "w+");
    for(; m<k-1; m++)
      fprintf(users, "%04d %-20s %-20s,%-20s %05d %s\n", id[m], username[m], nameOfUser[m], password[m], cash[m], type[m]);

    time_t t = time(NULL);
    struct tm time =*localtime(&t);

    FILE *data = fopen("data.txt", "a+");
    fprintf(data, "%04d %-20s,%04d %04d %4d-%02d-%02d %02d:%02d %4d-%02d-%02d %02d:%02d:%02d\n",
    userID, nameOfMovie[movieIndex-1], duration[movieIndex-1], price[movieIndex-1], year[movieIndex-1], month[movieIndex-1], day[movieIndex-1], hour[movieIndex-1], minute[movieIndex-1],
     time.tm_year+1900, time.tm_mon+1, time.tm_mday, time.tm_hour, time.tm_min, time.tm_sec);

     FILE *movies = fopen("Movies.txt", "w+");
     capacity[movieIndex-1]-=1;
     for (int j = 0; j < i-1; j++)
       fprintf(movies, "%04d %-20s,%04d %04d %04d %4d-%02d-%02d %02d:%02d\n",
       j+1, nameOfMovie[j], duration[j], capacity[j], price[j], year[j], month[j], day[j], hour[j], minute[j]);

    free(duration);
    free(capacity);
    free(price);
    free(year);
    free(month);
    free(day);
    free(hour);
    free(minute);
    fclose(data);
    fclose(users);
    fclose(movies);
    printf("* Your buy is successful!\n");
    user_menu(userID);

  }else if(select==2){
    fclose(users);
    fclose(movies);
    MoviesList(userID);
  }
    else{
      printf("! Your request was not accepted.\n");
      user_menu(userID);
    }
}

void TicketsList(int userID){
  FILE *data = fopen("data.txt", "r");
  int *id = malloc(10000*sizeof(int));
  char *nameOfMovie[21][1000];
  int *duration = malloc(10000*sizeof(int));
  int *price = malloc(10000*sizeof(int));
  int *year = malloc(10000*sizeof(int));
  int *month = malloc(10000*sizeof(int));
  int *day = malloc(10000*sizeof(int));
  int *hour = malloc(10000*sizeof(int));
  int *minute = malloc(10000*sizeof(int));
  int *buyYear = malloc(10000*sizeof(int));
  int *buyMonth = malloc(10000*sizeof(int));
  int *buyDay = malloc(10000*sizeof(int));
  int *BuyHour = malloc(10000*sizeof(int));
  int *buyMinute = malloc(10000*sizeof(int));
  int *buySec = malloc(10000*sizeof(int));
  int select;
  int i=0, j=0;
  printf("--------------------------------------------------------------------\n");
  printf("# List Of Tickets :\n\n");
  if(data == NULL)
    printf("*** You Have not any ticket yet ***\n\n");
  else{
    printf("Name OF Movie\t     Duration  Price  Date \t  Time   Buy Date    Buy Time\n");
    while(!feof(data)){
      fscanf(data,"%d %[^,],%d %d %d-%d-%d %d:%d %d-%d-%d %d:%d:%d",
      &id[i], nameOfMovie[i], &duration[i], &price[i], &year[i], &month[i], &day[i], &hour[i], &minute[i], &buyYear[i], &buyMonth[i], &buyDay[i], &BuyHour[i], &buyMinute[i], &buySec[i]);
      i++;
    }
    for (; j < i-1; j++) {
      if(id[j]==userID)
        printf("%-20s %04d      %04d   %4d-%02d-%02d  %02d:%02d  %4d-%02d-%02d  %02d:%02d:%02d\n",
        nameOfMovie[j], duration[j], price[j], year[j], month[j], day[j], hour[j], minute[j], buyYear[j], buyMonth[j], buyDay[j], BuyHour[j], buyMinute[j], buySec[j]);
    }
    free(id);
    free(duration);
    free(price);
    free(year);
    free(month);
    free(day);
    free(hour);
    free(minute);
    free(buyYear);
    free(buyMonth);
    free(buyDay);
    free(BuyHour);
    free(buyMinute);
    free(buySec);
    fclose(data);
  }
  printf("\n0.Back : \n");
  printf("-> Enter the command : ");
  scanf("%d", &select);
  if(select == 0)
    user_menu(userID);
  else{
    printf("! Your request was not accepted.\n");
    user_menu(userID);
  }
}

void PaymentsList(int userID){
  FILE *data = fopen("data.txt", "r");
  int *id = malloc(10000*sizeof(int));
  char nameOfMovie[21][1000];
  int *price = malloc(10000*sizeof(int));
  int *buyYear = malloc(10000*sizeof(int));
  int *buyMonth = malloc(10000*sizeof(int));
  int *buyDay = malloc(10000*sizeof(int));
  int *BuyHour = malloc(10000*sizeof(int));
  int *buyMinute = malloc(10000*sizeof(int));
  int *buySec = malloc(10000*sizeof(int));
  int select;
  int i=0, j=0;
  printf("--------------------------------------------------------------------\n");
  printf("# List Of Payments :\n\n");
  if(data == NULL)
    printf("*** N0 Payments have been done yet ***\n\n");
  else{
    printf("userID\tName OF Movie\t     Price  Buy Date\tBuy Time\n");
    while(!feof(data)){
      fscanf(data,"%d %[^,],%*d %d %*d-%*d-%*d %*d:%*d %d-%d-%d %d:%d:%d",
      &id[i], nameOfMovie[i], &price[i], &buyYear[i], &buyMonth[i], &buyDay[i], &BuyHour[i], &buyMinute[i], &buySec[i]);
      i++;
    }
    for (; j < i-1; j++) {
        printf("%04d    %-20s %04d   %4d-%02d-%02d  %02d:%02d:%02d\n",
        id[j], nameOfMovie[j], price[j], buyYear[j], buyMonth[j], buyDay[j], BuyHour[j], buyMinute[j], buySec[j]);
    }
    free(id);
    free(price);
    free(buyYear);
    free(buyMonth);
    free(buyDay);
    free(BuyHour);
    free(buyMinute);
    free(buySec);
    fclose(data);
  }
  printf("\n0.Back : \n");
  printf("-> Enter the command : ");
  scanf("%d", &select);
  if(select == 0)
    user_menu(userID);
  else{
    printf("! Your request was not accepted.\n");
    user_menu(userID);
  }
}

void AddMovies(int userID){
  FILE *movies = fopen("Movies.txt", "a");
  char name[20];
  int duration;
  int capacity;
  int price;
  int year;
  int month;
  int day;
  int hour;
  int minute;
  int id=1;
  char c;
  while((c=getc(movies)) != EOF)
    if(c=='\n')
      id++;

  printf("--------------------------------------------------------------------\n");
  printf("# Add new movie :\n");

  printf("-> Name : ");
  scanf("%*c%[^\n]", name);

  printf("-> Duration (min) : ");
  scanf("%d", &duration);

  printf("-> Capacity : ");
  scanf("%d", &capacity);

  printf("-> Price ($) : ");
  scanf("%d", &price);

  printf("-> Date : ");
  scanf("%d-%d-%d", &year, &month, &day);

  printf("-> Time : ");
  scanf("%d:%d", &hour, &minute);

  fprintf(movies, "%04d %-20s,%04d %04d %04d %4d-%02d-%02d %02d:%02d\n", id, name, duration, capacity, price, year, month, day, hour, minute);
  fclose(movies);
  printf("* New movie was added successful!\n");
  user_menu(userID);
}

void DeleteMovie(int userID){
  FILE *movies = fopen("Movies.txt", "r");
  printf("--------------------------------------------------------------------\n");
  printf("# Delete Movie :\n");
  if(movies == NULL){
    printf("*** N0 movies have been added yet ***\n\n");
  }else{
    char name[20][1000];
    int *duration = malloc(10000*sizeof(int));
    int *capacity = malloc(10000*sizeof(int));
    int *price = malloc(10000*sizeof(int));
    int *year = malloc(10000*sizeof(int));
    int *month = malloc(10000*sizeof(int));
    int *day = malloc(10000*sizeof(int));
    int *hour = malloc(10000*sizeof(int));
    int *minute = malloc(10000*sizeof(int));
    int select;
    int i=0, j=0;
    printf("Index\tName\t\t     Duration\t\t Capacity\t\t Price\t\t Date\t\t Time\n");
    while(!feof(movies)){
      fscanf(movies,"%*d %[^,],%d %d %d %d-%d-%d %d:%d",
      name[i], &duration[i], &capacity[i], &price[i], &year[i], &month[i], &day[i], &hour[i], &minute[i]);
      i++;
    }
    for(; j<i-1; j++){
      printf("%04d \t%-20s %d\t\t %d\t\t\t %d\t\t %4d-%02d-%02d\t %02d:%02d\n",
      (j+1), name[j], duration[j], capacity[j], price[j], year[j], month[j], day[j], hour[j], minute[j]);
    }
    printf("\n0.Back : \n");
    printf("1.Delet Movie : \n");
    printf("-> Enter the command : ");
    scanf("%d", &select);
    if(select == 0)
      MoviesList(userID);
    if(select == 1){
      int pos;
      printf("-> Enter the index of movie you want delete : ");
      scanf("%d", &pos);
      for(int k=pos-1; k<i; k++){
          strcpy(name[k], name[k+1]);
          duration[k] = duration[k+1];
          capacity[k] = capacity[k+1];
          price[k] = price[k+1];
          year[k] = year[k+1];
          month[k] = month[k+1];
          day[k] = day[k+1];
          hour[k] = hour[k+1];
          minute[k] = minute[k+1];
      }
      FILE *movies = fopen("Movies.txt", "w+");
      for (int m = 0; m < i-2; m++)
        fprintf(movies, "%04d %-20s,%04d %04d %04d %4d-%02d-%02d %02d:%02d\n",
        m+1, name[m], duration[m], capacity[m], price[m], year[m], month[m], day[m], hour[m], minute[m]);

      free(duration);
      free(capacity);
      free(price);
      free(year);
      free(month);
      free(day);
      free(hour);
      free(minute);
      fclose(movies);
      printf("* Done!\n");
      DeleteMovie(userID);
    }
  }
}

void EditeMovie(int userID){
  printf("--------------------------------------------------------------------\n");
  printf("# Edite Movie :\n");
  FILE *movies = fopen("Movies.txt", "r");
  if(movies == NULL){
    printf("*** N0 movies have been added yet ***\n\n");
  }else{
    char name[21][1000];
    int *duration = malloc(10000*sizeof(int));
    int *capacity = malloc(10000*sizeof(int));
    int *price = malloc(10000*sizeof(int));
    int *year = malloc(10000*sizeof(int));
    int *month = malloc(10000*sizeof(int));
    int *day = malloc(10000*sizeof(int));
    int *hour = malloc(10000*sizeof(int));
    int *minute = malloc(10000*sizeof(int));
    int select;
    int i=0, j=0;
    printf("Index\tName\t\t     Duration\t\t Capacity\t\t Price\t\t Date\t\t Time\n");
    while(!feof(movies)){
      fscanf(movies,"%*d %[^,],%d %d %d %d-%d-%d %d:%d",
      name[i], &duration[i], &capacity[i], &price[i], &year[i], &month[i], &day[i], &hour[i], &minute[i]);
      i++;
    }
    for(; j<i-1; j++){
      printf("%04d \t%-20s %d\t\t %d\t\t\t %d\t\t %4d-%02d-%02d\t %02d:%02d\n",
      (j+1), name[j], duration[j], capacity[j], price[j], year[j], month[j], day[j], hour[j], minute[j]);
    }
    printf("\n0.Back : \n");
    printf("1.Edite Movie : \n");
    printf("-> Enter the command : ");
    scanf("%d", &select);
    if(select == 0)
      MoviesList(userID);
    if(select == 1){
      int index;
      int item;
      printf("-> Enter the index of movie you want edite : ");
      scanf("%d", &index);
      char nameOfSelected[21];
      strcpy(nameOfSelected, name[index-1]);
      printf("-> Choose item you want edite :\n");
      printf("1.Name : \n");
      printf("2.Duration : \n");
      printf("3.Capacity : \n");
      printf("4.Price : \n");
      printf("5.Date : \n");
      printf("6.Time : \n");
      printf("-> Enter the command : ");
      scanf("%d", &item);

      if(item==1){
        printf("-> Enter New Name : ");
        scanf("%*c%[^\n]", name[index-1]);
      }
      if(item==2){
        printf("-> Enter New Duration (min) : ");
        scanf("%d", &duration[index-1]);
      }
      if(item==3){
        printf("-> Enter New Capacity : ");
        scanf("%d", &capacity[index-1]);
      }
      if(item==4){
        printf("-> Enter New Price ($) : ");
        scanf("%d", &price[index-1]);
      }
      if(item==5){
        printf("-> Enter New Date (YYY-MM-DD) : ");
        scanf("%d-%d-%d", &year[index-1], &month[index-1], &day[index-1]);
      }
      if(item==6){
        printf("-> Enter New Time (24:00 HOUR FORMAT): ");
        scanf("%d:%d", &hour[index-1], &minute[index-1]);
      }
      FILE *movies = fopen("Movies.txt", "w+");
      for (int j = 0; j < i-1; j++)
        fprintf(movies, "%04d %-20s,%04d %04d %04d %4d-%02d-%02d %02d:%02d\n",
        j+1, name[j], duration[j], capacity[j], price[j], year[j], month[j], day[j], hour[j], minute[j]);
      fclose(movies);

      FILE *data = fopen("data.txt", "r");
      int *userid = malloc(10000*sizeof(int));
      char *nameOfMovie[21][1000];
      int *dur = malloc(10000*sizeof(int));
      int *p = malloc(10000*sizeof(int));
      int *y = malloc(10000*sizeof(int));
      int *mon = malloc(10000*sizeof(int));
      int *d = malloc(10000*sizeof(int));
      int *h = malloc(10000*sizeof(int));
      int *min = malloc(10000*sizeof(int));
      int *buyY = malloc(10000*sizeof(int));
      int *buyMon = malloc(10000*sizeof(int));
      int *buyD = malloc(10000*sizeof(int));
      int *BuyH = malloc(10000*sizeof(int));
      int *buyMin = malloc(10000*sizeof(int));
      int *buyS = malloc(10000*sizeof(int));
      int m=0,n=0,x=0;

      while(!feof(data)){
        fscanf(data,"%d %[^,],%d %d %d-%d-%d %d:%d %d-%d-%d %d:%d:%d",
        &userid[m], nameOfMovie[m], &dur[m], &p[m], &y[m], &mon[m], &d[m], &h[m], &min[m], &buyY[m], &buyMon[m], &buyD[m], &BuyH[m], &buyMin[m], &buyS[m]);
        m++;
      }

      for (; n < m-1; n++) {
        if(strcmp(nameOfMovie[n], nameOfSelected)==0)
        {
          strcpy(nameOfMovie[n], name[index-1]);
          dur[n] =  duration[index-1];
          y[n] = year[index-1];
          mon[n] = month[index-1];
          d[n] = day[index-1];
          h[n] = hour[index-1];
          min[n] = minute[index-1];
        }
      }
      data = fopen("data.txt", "w+");
      for (; x < m-1; x++) {
        fprintf(data, "%04d %-20s,%04d %04d %4d-%02d-%02d %02d:%02d %4d-%02d-%02d %02d:%02d:%02d\n",
        userid[x], nameOfMovie[x], dur[x], p[x], y[x], mon[x], d[x], h[x], min[x], buyY[x], buyMon[x], buyD[x], BuyH[x], buyMin[x], buyS[x]);
      }
      free(userid);
      free(dur);
      free(p);
      free(y);
      free(mon);
      free(d);
      free(h);
      free(min);
      free(buyY);
      free(buyMon);
      free(buyD);
      free(BuyH);
      free(buyMin);
      free(buyS);
      free(duration);
      free(capacity);
      free(price);
      free(year);
      free(month);
      free(day);
      free(hour);
      free(minute);
      fclose(data);
      printf("* Done!\n");
      EditeMovie(userID);
    }
  }
}
