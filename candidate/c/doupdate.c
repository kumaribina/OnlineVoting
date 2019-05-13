#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include "ccgi.h"

#define DBFILE "../data/candidate.txt"
#define TEMPFILE "../data/temp.txt"
#define LIST_PATH "../../cgi-bin/candidate/list.cgi"
#define LIST_URL "http://localhost/cgi-bin/candidate/list.cgi"

int update(CGI_varlist *varlist);

struct candidate
{
    char id[30];
    char firstName[50];
    char lastName[50];
    char gender[10];
    char mobile[20];
    char email[50];
    char fatherName[50];
    char dateOfBirth[15];
    char party[50];
    char symbol[20];
    char address[100];

} candidate;

int main(int argc, char **argv)
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
        update(varlist);
    }
    else if (strcmp(value[0], "Back") == 0)
    {
        printf("<META HTTP-EQUIV=refresh CONTENT=\"1;URL=%s\">\n", LIST_URL);
    }

    CGI_free_varlist(varlist);
    return 0;
}

int update(CGI_varlist *varlist)
{
    FILE *fp, *fpt;
    CGI_value *value;

    
    printf("<html>"
           "<head>");
    printf("<title>Update Candidate</title>"
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

    while (fread(&candidate, sizeof(candidate), 1, fp) == 1)
    {
        value = CGI_lookup_all(varlist, "id");
        if (strcmp(value[0], candidate.id) == 0)
        {
            strcpy(candidate.firstName, CGI_lookup_all(varlist, "firstName")[0]);
            strcpy(candidate.lastName, CGI_lookup_all(varlist, "lastName")[0]);
            strcpy(candidate.mobile, CGI_lookup_all(varlist, "mobile")[0]);
            strcpy(candidate.email, CGI_lookup_all(varlist, "email")[0]);
            strcpy(candidate.fatherName, CGI_lookup_all(varlist, "fatherName")[0]);
            strcpy(candidate.dateOfBirth, CGI_lookup_all(varlist, "dateOfBirth")[0]);
            strcpy(candidate.address, CGI_lookup_all(varlist, "address")[0]);
            strcpy(candidate.party, CGI_lookup_all(varlist, "party")[0]);
            strcpy(candidate.symbol, CGI_lookup_all(varlist, "symbol")[0]);
            //strcpy(voter.gender,  voter.gender);
            // strcpy(voter.id, CGI_lookup_all(varlist, "firstName")[0]);

            fwrite(&candidate, sizeof(candidate), 1, fpt);
            }
            else
            {
                fwrite(&candidate, sizeof(candidate), 1, fpt);
                //printf("<p>Deleting the Voter with Voter/Aadhaar ID = %s and Name = %s %s</p>", voter.id, voter.firstName, voter.lastName);
            }
    }

    fclose(fpt);
    fclose(fp);
    remove(DBFILE);
    rename(TEMPFILE, DBFILE);
    printf("<p>Successfully Updated the candidate record.</p>");
    printf("<p>Click back to see the updated candidate record.</p>");
    printf("<FORM ACTION=\"%s\" METHOD=\"POST\">", LIST_PATH);
    printf("<input type=\"submit\" value=\"Back\" name=\"operation\"/>");
    printf("</FORM>");
    printf("</body></html>");
    return 0;
}