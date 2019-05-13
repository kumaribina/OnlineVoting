#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define MAX_NUMBER_CANDIDATE 20
#define DBFILE "../data/election.txt"
#define TEMPFILE "../data/temp.txt"
#define CANDIDATE_DBFILE "../data/candidate.txt"
#define VOTES_PATH "../../cgi-bin/user/vote.cgi"
#define VOTES_URL "http://localhost/cgi-bin/user/vote.cgi"

struct election
{
    char eid[30];
    char ename[50];
    char description[100];
    char etype[15];
    char startDate[15];
    char endsDate[15];
    char resultDate[15];
    struct vote
    {
        char candidateID[30];
        char voteCount[20];
        char isEmpty[3];
    } vote[MAX_NUMBER_CANDIDATE];
} election;

int main()
{

    FILE *fp;

    printf("%s%c%c\n",
           "Content-Type:text/html;charset=iso-8859-1", 13, 10);
    printf("<html>"
           "<head>");

    printf("<title>List of available Election</title>"
           "</head>"
           "<body>");

    fp = fopen(DBFILE, "rb");
    if (fp == NULL)
    {
        printf("<p>Oh dear, something went wrong with connecting database! %s</p>", strerror(errno));
        return 0;
    }

    
    printf("<table border=\"1\">"
           "<tr>"
           "<th>Election Name</th>"
           "<th>Election Description</th>"
           "<th>Election Type</th>"
           "<th>Ends Date</th>"
           "<th>Message</th>"
           "<th>Click</th>"
           "</tr>");

    while (fread(&election, sizeof(election), 1, fp) == 1)
    {
        printf("<FORM ACTION=\"%s\" METHOD=\"POST\">", VOTES_URL);
        printf("<tr>");
        printf("<input type=\"hidden\" name=\"selected\" value=\"%s\" />", election.eid);
        printf("<td>%s</td>", election.ename);
        printf("<td>%s</td>", election.description);
        printf("<td>%s</td>", election.etype);
        printf("<td>%s</td>", election.endsDate);
        printf("<td><b style=\"color:green;\">Election is OPEN</b>.Click <b>Candidate List</b> to see Contestants.</td>");
        printf("<td><input type=\"submit\" value=\"Candidate List\"/></td>");
        //printf("<td>%s</td>", election.dateOfBirth);
        //printf("<td>%s</td>", election.address);
        printf("</tr>");
        printf("</form>");
    }
    printf("</table>");
    
    //print ends
    printf("</body></html>");
    fclose(fp);
    return 0;
}