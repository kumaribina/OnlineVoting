#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include "ccgi.h"

#define MAX_NUMBER_CANDIDATE 20
#define MAX_NUMBER_VOTER 30
#define DBFILE "../data/election.txt"
#define CANDIDATE_DBFILE "../data/candidate.txt"
#define TEMPFILE "../data/temp.txt"
#define LIST_URL "../../cgi-bin/election/list.cgi"
#define ADD_URL "http://localhost/cgi-bin/election/add.cgi"
#define DO_UPDATE_URL "../../cgi-bin/election/doupdate.cgi"

int delete(CGI_value *val);
int edit(CGI_value *val);

struct election
{
    char eid[30];
    char ename[50];
    char description[100];
    char etype[15];
    char startDate[15];
    char endsDate[15];
    char resultDate[15];
    struct ecandidate
    {
        char candidateID[30];
        char voteCount[20];
        char isEmpty[3];
    } ecandidate[MAX_NUMBER_CANDIDATE];
    struct evoter
    {
        char voterID[30];
        char votedCandidateID[30];
        char voteDateTime[15];
        char isEmpty[3];
    } evoter[MAX_NUMBER_VOTER];
} election;

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
    printf("<title>Delete Election</title>"
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

    while (fread(&election, sizeof(election), 1, fp) == 1)
    {
        for (i = 0; val[i] != 0; i++)
        {
            if (strcmp(val[i], election.eid) != 0)
            {
                fwrite(&election, sizeof(election), 1, fpt);
            }
            else
            {
                printf("<p>Deleting the Election with ID = %s and Name = %s</p>", election.eid, election.ename);
            }
        }
    }

    fclose(fpt);
    fclose(fp);
    remove(DBFILE);
    rename(TEMPFILE, DBFILE);
    printf("<p>Successfully deleted %d Election record.</p>", i);
    printf("<p>Click back to see the updated Election record.</p>");
    printf("<FORM ACTION=\"%s\" METHOD=\"POST\">", LIST_URL);
    printf("<input type=\"submit\" value=\"Back\" name=\"operation\"/>");
    printf("</FORM>");
    printf("</body></html>");
    return 0;
}

int edit(CGI_value *val)
{
    FILE *fp, *fpc;
    int i;

    //printf("%s%c%c\n",
       //    "Content-Type:text/html;charset=iso-8859-1", 13, 10);
    printf("<html>"
           "<head>");
    printf("<title>Edit Election</title>"
           "</head>"
           "<body>");
    
    fp = fopen(DBFILE, "rb");
    if (fp == NULL)
    {
        printf("<p>Oh dear, something went wrong with connecting database! %s</p>", strerror(errno));
        exit(1);
    }

    fpc = fopen(CANDIDATE_DBFILE, "rb");
    if (fpc == NULL)
    {
        printf("<p>Oh dear, something went wrong with connecting database! %s</p>", strerror(errno));
        return 0;
    }

    for (i = 0; val[i] != 0; i++);
    if (i > 1) {
        printf("<p>Only One record can be updated, Go back and select one.</p>");
        printf("<FORM ACTION=\"%s\" METHOD=\"POST\">", LIST_URL);
        printf("<input type=\"submit\" value=\"Back\" name=\"operation\"/>");
        printf("</FORM>");
        printf("</body></html>");
        exit(1);
    }
    printf("<h2>Update Election</h2>");
    printf("<FORM ACTION=\"%s\" METHOD=\"POST\">", DO_UPDATE_URL);

    while (fread(&election, sizeof(election), 1, fp) == 1)
    {
        for (i = 0; val[i] != 0; i++)
        {

            if (strcmp(val[i], election.eid) == 0)
            {

                printf("<table border=\"1\">");
                printf("<tr><TD>Election ID:</TD><td><input type=\"text\" name=\"id\" value=\"%s\" readonly=\"readonly\"/></td></tr>", election.eid);

                //printf("<tr><td>Election ID:</td><td> <input type=\"text\" name=\"eid\"/> </td></tr>", election.firstName);
                printf("<tr><td>Name:</td><td> <input type=\"text\" name=\"ename\" value=\"%s\"/> </td></tr>", election.ename);
                printf("<tr><TD>Description:</TD> <TD> <input type=\"text\" name=\"description\" value=\"%s\"/> </TD></tr>", election.description);
                printf("<tr><TD>Election Type:</TD> <TD> <input type=\"text\" name=\"etype\" value=\"%s\"/> </TD></tr>", election.etype);
                printf("<tr><TD>Start Date:</TD> <TD> <input type=\"text\" name=\"startDate\" value=\"%s\"/> </TD></tr>", election.startDate);
                printf("<tr><TD>Ends Date:</TD> <TD> <input type=\"text\" name=\"endsDate\" value=\"%s\"/> </TD></tr>", election.endsDate);
                printf("<tr><TD>Results Date:</TD> <TD> <input type=\"text\" name=\"resultDate\" value=\"%s\"/></tr>", election.resultDate);

                printf("<tr>"
                       "<td>Select candidates</td>"
                       "<td>"
                       "<table border=\"1\">"
                       "<tr>"
                       "<th>Select</th>"
                       "<th>Voter/Aadhaar ID</th>"
                       "<th>First Name</th>"
                       "<th>Last Name</th>"
                       "<th>Gender</th>"
                       "<th>Party</th>"
                       "<th>Symbol</th>"
                       "</tr>");

                while (fread(&candidate, sizeof(candidate), 1, fpc) == 1)
                {
                    printf("<tr>");
                    if (strcmp(candidate.id, election.eid) == 0)
                        printf("<td><input type=\"checkbox\" name=\"candidateID\" value=\"%s\" checked/></td>", candidate.id);
                    else
                        printf("<td><input type=\"checkbox\" name=\"candidateID\" value=\"%s\"/></td>", candidate.id);
                    printf("<td>%s</td>", candidate.id);
                    printf("<td>%s</td>", candidate.firstName);
                    printf("<td>%s</td>", candidate.lastName);
                    printf("<td>%s</td>", candidate.gender);
                    printf("<td>%s</td>", candidate.party);
                    printf("<td>%s</td>", candidate.symbol);
                    printf("</tr>");
                }
                printf("</table>"
                       "</td>"
                       "</tr>");
                printf("</table>");
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
    fclose(fpc);
    return 0;
}