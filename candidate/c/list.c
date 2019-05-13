#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define DBFILE "../data/candidate.txt"
#define EDIT_URL "../../cgi-bin/candidate/update.cgi"

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
    printf("<style>table{font-family:arial,sans-serif;border-collapse:collapse;width:100%}"
           "td,th{border:1px solid #ddd;text-align:left;padding:8px}"
           "tr:nth-child(even){background-color:#ddd}"
           "</style>");
    printf("<title>Candidates List</title>"
           "</head>"
           "<body>");

    fp = fopen(DBFILE, "rb");
    if(fp == NULL)
    {
        printf("<p>Oh dear, something went wrong with connecting database! %s</p>", strerror(errno));
        return 0;
    }
    printf("<FORM ACTION=\"%s\" METHOD=\"POST\">", EDIT_URL);
    printf("<table>"
           "<tr>"
           "<th>Select</th>"
           "<th>Voter/Aadhaar ID</th>"
           "<th>First Name</th>"
           "<th>Last Name</th>"
           "<th>Gender</th>"
           "<th>Mobile Number</th>"
           "<th>Email ID</th>"
           "<th>Father Name</th>"
           "<th>Date of Birth</th>"
           "<th>Address</th>"
           "<th>Party</th>"
           "<th>Symbol</th>"
           "</tr>");

    while (fread(&candidate, sizeof(candidate), 1, fp) == 1)
    {
        printf("<tr>");
        printf("<td><input type=\"checkbox\" name=\"selected\" value=\"%s\" /></td>", candidate.id);
        printf("<td>%s</td>", candidate.id);
        printf("<td>%s</td>", candidate.firstName);
        printf("<td>%s</td>", candidate.lastName);
        printf("<td>%s</td>", candidate.gender);
        printf("<td>%s</td>", candidate.mobile);
        printf("<td>%s</td>", candidate.email);
        printf("<td>%s</td>", candidate.fatherName);
        printf("<td>%s</td>", candidate.dateOfBirth);
        printf("<td>%s</td>", candidate.address);
        printf("<td>%s</td>", candidate.party);
        printf("<td>%s</td>", candidate.symbol);
        printf("</tr>");
    }
    printf("</table>");
    //printf("<input type=\"hidden\" value=\"empty\" name=\"dummy\">");
    printf("<input type=\"submit\" value=\"Add\" name=\"operation\"/>");
    printf("<input type=\"submit\" value=\"Edit\" name=\"operation\"/>");
    printf("<input type=\"submit\" value=\"Delete\" name=\"operation\"/>");
    //print ends
    printf("</form>");
    printf("</body></html>");
    fclose(fp);
    return 0;
}