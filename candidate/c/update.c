#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include "ccgi.h"

#define DBFILE "../data/candidate.txt"
#define TEMPFILE "../data/temp.txt"
#define LIST_PATH "../../cgi-bin/candidate/list.cgi"
#define ADD_URL "http://localhost/addCandidate.html"
#define UPDATE_PATH "../../cgi-bin/candidate/doupdate.cgi"

int delete(CGI_value *val);
int edit(CGI_value *val);

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

int main(int argc, char **argv) {
    CGI_varlist *varlist;
    //const char *name;
    CGI_value  *value;

    printf("%s%c%c\n",
           "Content-Type:text/html;charset=iso-8859-1", 13, 10);
    if ((varlist = CGI_get_all(0)) == 0) {
        printf("<p>No CGI data received</p>\r\n");
        exit(1);
    }

    
    /* output all values of all variables and cookies */
    value = CGI_lookup_all(varlist, "operation");
    if (strcmp(value[0], "Delete") == 0) {
        value = CGI_lookup_all(varlist, "selected");
        delete(value);
    }
    else if (strcmp(value[0], "Add") == 0)
    {
        printf("<META HTTP-EQUIV=refresh CONTENT=\"1;URL=%s\">\n", ADD_URL);
    }
    else if (strcmp(value[0], "Edit") == 0) {
        value = CGI_lookup_all(varlist, "selected");
        edit(value);
    }
    

        CGI_free_varlist(varlist);
    return 0;
}

int delete(CGI_value *val)
{
    int i = 0;
    FILE *fp, *fpt;

    //printf("%s%c%c\n",
      //     "Content-Type:text/html;charset=iso-8859-1", 13, 10);
    printf("<html>"
           "<head>");
    printf("<title>Delete Candidate</title>"
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
        for (i = 0; val[i] != 0; i++)
        {
            if (strcmp(val[i], candidate.id) != 0)
            {
                fwrite(&candidate, sizeof(candidate), 1, fpt);
            }
            else
            {
                printf("<p>Deleting the Candidate with Voter/Aadhaar ID = %s and Name = %s %s</p>", candidate.id, candidate.firstName, candidate.lastName);
            }
        }
    }

    fclose(fpt);
    fclose(fp);
    remove(DBFILE);
    rename(TEMPFILE, DBFILE);
    printf("<p>Successfully deleted %d candidate record.</p>", i);
    printf("<p>Click back to see the updated candidate record.</p>");
    printf("<FORM ACTION=\"%s\" METHOD=\"POST\">", LIST_PATH);
    printf("<input type=\"submit\" value=\"Back\" name=\"operation\"/>");
    printf("</FORM>");
    printf("</body></html>");
    return 0;
}

int edit(CGI_value *val)
{
    FILE *fp;
    int i;

    //printf("%s%c%c\n",
       //    "Content-Type:text/html;charset=iso-8859-1", 13, 10);
    printf("<html>"
           "<head>");
    printf("<title>Edit Candidate</title>"
           "</head>"
           "<body>");

    fp = fopen(DBFILE, "rb");
    if (fp == NULL)
    {
        printf("<p>Oh dear, something went wrong with connecting database! %s</p>", strerror(errno));
        exit(1);
    }

    for (i = 0; val[i] != 0; i++);
    if (i > 1) {
        printf("<p>Only One record can be updated, Go back and select one.</p>");
        printf("<FORM ACTION=\"%s\" METHOD=\"POST\">", LIST_PATH);
        printf("<input type=\"submit\" value=\"Back\" name=\"operation\"/>");
        printf("</FORM>");
        printf("</body></html>");
        exit(1);
    }
    printf("<FORM ACTION=\"%s\" METHOD=\"POST\">", UPDATE_PATH);
    printf("<table BORDER=\"1\">");

    while (fread(&candidate, sizeof(candidate), 1, fp) == 1)
    {
        for (i = 0; val[i] != 0; i++)
        {

            if (strcmp(val[i], candidate.id) == 0)
            {

                printf("<tr><TD>Voter/Aadhaar ID:</TD><td><input type=\"text\" name=\"id\" value=\"%s\" readonly=\"readonly\"/></td></tr>", candidate.id);
                printf("<tr><TD>First Name:</TD><td><input type=\"text\" name=\"firstName\" value=\"%s\"/></td></tr>", candidate.firstName);
                printf("<tr><TR><TD>Last Name:</TD><td><input type=\"text\" name=\"lastName\" value=\"%s\"/></td></tr>", candidate.lastName);
                //printf("<tr><TD>Gender:</TD><td><input type=\"radio\" name=\"gender\" value=\"%s\"/></td></tr>", candidate.gender);
                printf("<tr><TD>Mobile Number:</TD><td><input type=\"text\" name=\"mobile\" value=\"%s\"/></td></tr>", candidate.mobile);
                printf("<tr><TD>Email:</TD><td><input type=\"text\" name=\"email\" value=\"%s\"/></td></tr>", candidate.email);
                printf("<tr><TD>Father Name:</TD><td><input type=\"text\" name=\"fatherName\" value=\"%s\"/></td></tr>", candidate.fatherName);
                printf("<tr><TD>Date of Birth:</TD><td><input type=\"text\" name=\"dateOfBirth\" value=\"%s\"/></td></tr>", candidate.dateOfBirth);
                printf("<tr><TD>Address:</TD><td><input type=\"text\" name=\"address\" value=\"%s\"/></td></tr>", candidate.address);
                printf("<tr><TD>Party:</TD><td><input type=\"text\" name=\"party\" value=\"%s\"/></td></tr>", candidate.party);
                printf("<tr><TD>Symbol:</TD><td><input type=\"text\" name=\"symbol\" value=\"%s\"/></td></tr>", candidate.symbol);
            }

            
        }
    }
    printf("</table>");
    printf("<input type=\"submit\" value=\"Update\" name=\"operation\"/>");
    printf("<input type=\"submit\" value=\"Back\" name=\"operation\"/>");
    //print ends
    printf("</form>");
    printf("</body></html>");
    fclose(fp);
    return 0;
}