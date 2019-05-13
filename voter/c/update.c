#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include "ccgi.h"

#define DBFILE "../data/voter.txt"
#define TEMPFILE "../data/temp.txt"
#define LIST_VOTER_PATH "../../cgi-bin/voter/list.cgi"
#define ADD_VOTER_URL "http://localhost/addVoter.html"
#define UPDATE_VOTER_PATH "../../cgi-bin/voter/doupdate.cgi"

int delete_voter(CGI_value *val);
int edit_voter(CGI_value *val);

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
        delete_voter(value);
    }
    else if (strcmp(value[0], "Add") == 0)
    {
        printf("<META HTTP-EQUIV=refresh CONTENT=\"1;URL=%s\">\n", ADD_VOTER_URL);
    }
    else if (strcmp(value[0], "Edit") == 0) {
        value = CGI_lookup_all(varlist, "selected");
        edit_voter(value);
    }
    

        CGI_free_varlist(varlist);
    return 0;
}

int delete_voter(CGI_value *val)
{
    int i = 0;
    FILE *fp, *fpt;

    //printf("%s%c%c\n",
      //     "Content-Type:text/html;charset=iso-8859-1", 13, 10);
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
        for (i = 0; val[i] != 0; i++)
        {
            if (strcmp(val[i], voter.id) != 0)
            {
                fwrite(&voter, sizeof(voter), 1, fpt);
            }
            else
            {
                printf("<p>Deleting the Voter with Voter/Aadhaar ID = %s and Name = %s %s</p>", voter.id, voter.firstName, voter.lastName);
            }
        }
    }

    fclose(fpt);
    fclose(fp);
    remove(DBFILE);
    rename(TEMPFILE, DBFILE);
    printf("<p>Successfully deleted %d voter record.</p>", i);
    printf("<p>Click back to see the updated voter record.</p>");
    printf("<FORM ACTION=\"%s\" METHOD=\"POST\">", LIST_VOTER_PATH);
    printf("<input type=\"submit\" value=\"Back\" name=\"operation\"/>");
    printf("</FORM>");
    printf("</body></html>");
    return 0;
}

int edit_voter(CGI_value *val)
{
    FILE *fp;
    int i;

    //printf("%s%c%c\n",
       //    "Content-Type:text/html;charset=iso-8859-1", 13, 10);
    printf("<html>"
           "<head>");
    printf("<title>Edit Voter</title>"
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
        printf("<FORM ACTION=\"%s\" METHOD=\"POST\">", LIST_VOTER_PATH);
        printf("<input type=\"submit\" value=\"Back\" name=\"operation\"/>");
        printf("</FORM>");
        printf("</body></html>");
        exit(1);
    }
    printf("<FORM ACTION=\"%s\" METHOD=\"POST\">", UPDATE_VOTER_PATH);
    printf("<table BORDER=\"1\">");

    while (fread(&voter, sizeof(voter), 1, fp) == 1)
    {
        for (i = 0; val[i] != 0; i++)
        {
            
            if (strcmp(val[i], voter.id) == 0)
            {

                printf("<tr><TD>Voter/Aadhaar ID:</TD><td><input type=\"text\" name=\"id\" value=\"%s\" readonly=\"readonly\"/></td></tr>", voter.id);
                printf("<tr><TD>First Name:</TD><td><input type=\"text\" name=\"firstName\" value=\"%s\"/></td></tr>", voter.firstName);
                printf("<tr><TR><TD>Last Name:</TD><td><input type=\"text\" name=\"lastName\" value=\"%s\"/></td></tr>", voter.lastName);
                //printf("<tr><TD>Gender:</TD><td><input type=\"radio\" name=\"gender\" value=\"%s\"/></td></tr>", voter.gender);
                printf("<tr><TD>Mobile Number:</TD><td><input type=\"text\" name=\"mobile\" value=\"%s\"/></td></tr>", voter.mobile);
                printf("<tr><TD>Email:</TD><td><input type=\"text\" name=\"email\" value=\"%s\"/></td></tr>", voter.email);
                printf("<tr><TD>Father Name:</TD><td><input type=\"text\" name=\"fatherName\" value=\"%s\"/></td></tr>", voter.fatherName);
                printf("<tr><TD>Date of Birth:</TD><td><input type=\"text\" name=\"dateOfBirth\" value=\"%s\"/></td></tr>", voter.dateOfBirth);
                printf("<tr><TD>Address:</TD><td><input type=\"text\" name=\"address\" value=\"%s\"/></td></tr>", voter.address);
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