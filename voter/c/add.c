#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ccgi.h"
#include <errno.h>

#define DBFILE "../data/voter.txt"
#define TEMPFILE "../data/temp.txt"
#define VOTER_URL "../../cgi-bin/voter/list.cgi"

int add_voter();
int delete_voter();
int update_voter();
int edit_voter();
int list_voter();

struct voter
{
    char id[30];
    char firstName[50];
    char lastName[50];
    char gender[10];
    char mobile[20];
    char email[50];
    char fatherName[50];
    char dateOfBirth[15];
    char address[100];
} voter;


int main()
{

    CGI_varlist *varlist;
    const char *name;
    CGI_value  *value;
    FILE *fp;

    fp = fopen(DBFILE, "a");

    printf("%s%c%c\n",
           "Content-Type:text/html;charset=iso-8859-1", 13, 10);
    printf("<html>"
           "<head>");
    printf("<style>table{font-family:arial,sans-serif;border-collapse:collapse;width:100%}"
           "td,th{border:1px solid #ddd;text-align:left;padding:8px}"
           "tr:nth-child(even){background-color:#ddd}"
           "</style>");
    printf("<title>Add Voter</title>"
           "</head>"
           "<body>");
    if(fp == NULL)
    {
        printf("<p>Oh dear, something went wrong with connecting database! %s</p>", strerror(errno));
        return 0;
    }

    if ((varlist = CGI_get_all(0)) == 0)
    {
        printf("<p>Oh dear, Empty request! Nothing Added.</p>");
        fclose(fp);
        return 0;
    }

    for (name = CGI_first_name(varlist); name != 0; name = CGI_next_name(varlist))
    {
        value = CGI_lookup_all(varlist, name);

        if (strcmp(name,"firstName")==0)
            strcpy(voter.firstName, value[0]);
        else if (strcmp(name,"lastName")==0)
            strcpy(voter.lastName, value[0]);
        else if (strcmp(name,"mobile")==0)
            strcpy(voter.mobile, value[0]);
        else if (strcmp(name,"email")==0)
            strcpy(voter.email, value[0]);
        else if (strcmp(name,"fatherName")==0)
            strcpy(voter.fatherName, value[0]);
        else if (strcmp(name,"dateOfBirth")==0)
            strcpy(voter.dateOfBirth, value[0]);
        else if (strcmp(name,"address")==0)
            strcpy(voter.address, value[0]);
        else if (strcmp(name,"gender")==0)
            strcpy(voter.gender, value[0]);
        else if (strcmp(name,"id")==0)
            strcpy(voter.id, value[0]);
    }

    fwrite(&voter, sizeof(voter), 1, fp);
    fclose(fp);
    CGI_free_varlist(varlist);  /* free variable list */
    printf("<p>Added the Voter with Voter/Aadhaar ID = %s and Name = %s %s.</p>", voter.id, voter.firstName, voter.lastName);
    printf("<p>Click back to see the updated voter record.</p>");
    printf("<FORM ACTION=\"%s\" METHOD=\"POST\">", VOTER_URL);
    printf("<input type=\"submit\" value=\"Back\" name=\"operation\"/>");
    printf("</FORM>");
    printf("</body></html>");
    return 0;
}