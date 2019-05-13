#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include "ccgi.h"

#define DBFILE "../data/voter.txt"
#define TEMPFILE "../data/temp.txt"
#define LIST_VOTER_PATH "../../cgi-bin/voter/list.cgi"
#define LIST_VOTER_URL "http://localhost/cgi-bin/user/list.cgi"

int update_voter(CGI_varlist *varlist);

struct election
{
    char eid[30];
    char ename[50];
    char description[100];
    char etype[15];
    char startDate[15];
    char endsDate[15];
    char resultDate[15];
    struct candidate
    {
        char candidateID[30];
        char voteCount[20];
        char isEmpty[3];
    } candidate[MAX_NUMBER_CANDIDATE];
    struct voter
    {
        char voterID[30];
        char votedCandidateID[30];
        char voteDateTime[15];
        char isEmpty[3];
    } voter[MAX_NUMBER_VOTER];
} election;

int main(int argc, char **argv)
{
    CGI_varlist *varlist;
    const char *name;
    CGI_value *value;
    int i;

    fputs("Content-type: text/plain\r\n\r\n", stdout);
    if ((varlist = CGI_get_all(0)) == 0)
    {
        printf("No CGI data received\r\n");
        return 0;
    }

    /* output all values of all variables and cookies */

    for (name = CGI_first_name(varlist); name != 0;
         name = CGI_next_name(varlist))
    {
        value = CGI_lookup_all(varlist, 0);

        /* CGI_lookup_all(varlist, name) could also be used */

        for (i = 0; value[i] != 0; i++)
        {
            printf("%s [%d] = %s\r\n", name, i, value[i]);
        }
    }
    CGI_free_varlist(varlist); /* free variable list */
    return 0;
}

int main_(int argc, char **argv)
{
    CGI_varlist *varlist;
    CGI_value *value;

    printf("%s%c%c\n",
           "Content-Type:text/html;charset=iso-8859-1", 13, 10);
           
    if ((varlist = CGI_get_all(0)) == 0)
    {
        printf("No CGI data received\r\n");
        return 0;
    }

    value = CGI_lookup_all(varlist, "operation");
    if (strcmp(value[0], "Update") == 0)
    {
        update_voter(varlist);
    }
    else if (strcmp(value[0], "Back") == 0)
    {
        printf("<META HTTP-EQUIV=refresh CONTENT=\"1;URL=%s\">\n", LIST_VOTER_URL);
    }

    CGI_free_varlist(varlist);
    return 0;
}

int update_voter(CGI_varlist *varlist)
{
    FILE *fp, *fpt;
    CGI_value *value;

    
    printf("<html>"
           "<head>");
    printf("<title>Delete Voter</title>"
           "</head>"
           "<body>");

    fp = fopen(DBFILE, "rb");
    if (fp == NULL)
    {
        printf("<p>Oh dear, something went wrong with connecting database! %s</p>", strerror(errno));
        return 0;
    }
    fpt = fopen(TEMPFILE, "wb");
    if (fpt == NULL)
    {
        printf("<p>Oh dear, something went wrong with connecting database! %s</p>", strerror(errno));
        return 0;
    }
    
    while (fread(&voter, sizeof(voter), 1, fp) == 1)
    {
        value = CGI_lookup_all(varlist, "id");
        if (strcmp(value[0], voter.id) == 0)
        {
            strcpy(voter.firstName, CGI_lookup_all(varlist, "firstName")[0]);
            strcpy(voter.lastName, CGI_lookup_all(varlist, "lastName")[0]);
            strcpy(voter.mobile, CGI_lookup_all(varlist, "mobile")[0]);
            strcpy(voter.email, CGI_lookup_all(varlist, "email")[0]);
            strcpy(voter.fatherName, CGI_lookup_all(varlist, "fatherName")[0]);
            strcpy(voter.dateOfBirth, CGI_lookup_all(varlist, "dateOfBirth")[0]);
            strcpy(voter.address, CGI_lookup_all(varlist, "address")[0]);
            //strcpy(voter.gender,  voter.gender);
            // strcpy(voter.id, CGI_lookup_all(varlist, "firstName")[0]);

            fwrite(&voter, sizeof(voter), 1, fpt);
            }
            else
            {
                fwrite(&voter, sizeof(voter), 1, fpt);
                //printf("<p>Deleting the Voter with Voter/Aadhaar ID = %s and Name = %s %s</p>", voter.id, voter.firstName, voter.lastName);
            }
    }

    fclose(fpt);
    fclose(fp);
    remove(DBFILE);
    rename(TEMPFILE, DBFILE);
    printf("<p>Successfully Updated the voter record.</p>");
    printf("<p>Click back to see the updated voter record.</p>");
    printf("<FORM ACTION=\"%s\" METHOD=\"POST\">", LIST_VOTER_PATH);
    printf("<input type=\"submit\" value=\"Back\" name=\"operation\"/>");
    printf("</FORM>");
    printf("</body></html>");
    return 0;
}