#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define CANDIDATE_DBFILE "../data/candidate.txt"
#define CANDIDATE_DOADD_URL "../../cgi-bin/election/doadd.cgi"

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

int main()
{

    FILE *fp;

    printf("%s%c%c\n",
           "Content-Type:text/html;charset=iso-8859-1", 13, 10);
    printf("<html>"
           "<head>");
    printf("<title>Add Election</title>"
           "</head>"
           "<body>");

    fp = fopen(CANDIDATE_DBFILE, "rb");
    if (fp == NULL)
    {
        printf("<p>Oh dear, something went wrong with connecting database! %s</p>", strerror(errno));
        return 0;
    }
    printf("<h2>Add Election</h2>");
    printf("<FORM ACTION=\"%s\" METHOD=\"POST\">", CANDIDATE_DOADD_URL);
    printf("<table border=\"1\">"
           "<TR> <td>Election ID:</td><td> <input type=\"text\" name=\"eid\"/> </td></TR>"
           "<TR> <td>Name:</td><td> <input type=\"text\" name=\"ename\"/> </td></TR>"
           "<TR> <TD>Description:</TD> <TD> <input type=\"text\" name=\"description\"/> </TD> </TR>"
           "<TR> <TD>Election Type:</TD> <TD> <input type=\"text\" name=\"etype\"/> </TD> </TR>"
           "<TR> <TD>Start Date:</TD> <TD> <input type=\"text\" name=\"startDate\"/> </TD> </TR>"
           "<TR> <TD>Ends Date:</TD> <TD> <input type=\"text\" name=\"endsDate\"/> </TD> </TR>"
           "<TR> <TD>Results Date:</TD> <TD> <input type=\"text\" name=\"resultDate\"/> </TD> </TR>"

           "<tr>"
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

    while (fread(&candidate, sizeof(candidate), 1, fp) == 1)
    {
        printf("<tr>");
        printf("<td><input type=\"checkbox\" name=\"candidateID\" value=\"%s\" /></td>", candidate.id);
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
    printf("<input type=\"submit\" value=\"Add\" name=\"operation\"/>");
    //print ends
    printf("</form>");
    printf("</body></html>");
    fclose(fp);
    return 0;
}