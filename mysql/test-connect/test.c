/*
 * =====================================================================================
 *
 *       Filename:  test.cc
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  09/09/2012 09:03:08 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  dongyuchi (dongyuchi), dongyuchi@gmail.com
 *        Company:  UESTC
 *
 * =====================================================================================
 */
#include<mysql/mysql.h>
#include<stdio.h>
int main(){
    MYSQL mysql;
    MYSQL_RES *res;
    MYSQL_ROW row;
    char *query = "select * from worker";
    int t,r;
    mysql_init(&mysql);
    int m=0;
    if(!mysql_real_connect(&mysql,"localhost","root","ndsl435","company",0,NULL,0)){
        printf("Error connecting to database:%s\n",mysql_error(&mysql));
    }
    else{
        printf("Connected....\n");
    }

    t = mysql_query(&mysql,query);
    if(t){
        printf("Error making query:%s\n",mysql_error(&mysql));
    }
    else{
        printf("Query made...\n");
        res = mysql_use_result(&mysql);
        row = mysql_fetch_row(res);
        while(row > 0){
            //printf("%s\n",row["nid"]);
            for(t=0;t<mysql_num_fields(res);t++)
               printf("%s ",row[t]);
            printf("\n");
            row = mysql_fetch_row(res);
        }
        mysql_free_result(res);
    }
    mysql_close(&mysql);
    return 0;
}
