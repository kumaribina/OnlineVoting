#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define MAX_NUMBER_CANDIDATE 20
#define MAX_NUMBER_VOTER 30
#define DBFILE "../data/election.txt"
#define TEMPFILE "../data/temp.txt"
#define CANDIDATE_DBFILE "../data/candidate.txt"
#define UPDATE_URL "../../cgi-bin/election/update.cgi"

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

int main()
{

    FILE *fp, *fpc;
    int i = 0;
    printf("%s%c%c\n",
           "Content-Type:text/html;charset=iso-8859-1", 13, 10);
    printf("<html>"
           "<head>");

    printf("<title>Election List</title>"
           "</head>"
           "<body>");

    fp = fopen(DBFILE, "rb");
    if(fp == NULL)
    {
        printf("<p>Oh dear, something went wrong with connecting database! %s %s</p>", DBFILE, strerror(errno));
        return 0;
    }

    fpc = fopen(CANDIDATE_DBFILE, "rb");
    if (fpc == NULL)
    {
        printf("<p>Oh dear, something went wrong with connecting database! %s %s</p>", CANDIDATE_DBFILE, strerror(errno));
        return 0;
    }
    printf("<FORM ACTION=\"%s\" METHOD=\"POST\">", UPDATE_URL);
    printf("<table border=\"1\">"
           "<tr>"
           "<th>Select</th>"
           "<th>Election ID</th>"
           "<th>Election Name</th>"
           "<th>Election Description</th>"
           "<th>Election Type</th>"
           "<th>Candidates</th>"
           "<th>Start Date</th>"
           "<th>Ends Date</th>"
           "<th>Result Date</th>"
           "</tr>");

    while (fread(&election, sizeof(election), 1, fp) == 1)
    {
        printf("<tr>");
        printf("<td><input type=\"checkbox\" name=\"selected\" value=\"%s\" /></td>", election.eid);
        printf("<td>%s</td>", election.eid);
        printf("<td>%s</td>", election.ename);
        printf("<td>%s</td>", election.description);
        printf("<td>%s</td>", election.etype);

        //Print the candidates info with header here
        printf("<td><table border=\"1\">");
        printf("<tr>"
               "<th>Voter/Aadhaar ID</th>"
               "<th>Name</th>"
               "<th>Gender</th>"
               "<th>Party</th>"
               "<th>Symbol</th>"
               "<th>Vote Received</th>"
               "</tr>");
        for (i = 0; i < MAX_NUMBER_CANDIDATE; i++)
        {
            if (strcmp(election.ecandidate[i].isEmpty, "1") == 0)
            {
                while (fread(&candidate, sizeof(candidate), 1, fpc) == 1)
                {
                    if (strcmp(candidate.id, election.ecandidate[i].candidateID) == 0)
                    {
                        printf("<tr>");
                        printf("<td>%s</td>", election.ecandidate[i].candidateID);
                        printf("<td>%s %s</td>", candidate.firstName, candidate.lastName);
                        printf("<td>%s</td>", candidate.gender);
                        printf("<td>%s</td>", candidate.party);
                        printf("<td>%s</td>", candidate.symbol);
                        printf("<td>%s</td>", election.ecandidate[i].voteCount);
                        printf("</tr>");
                    }
                }
            }
            rewind(fpc); //set the candidate file pointer to start
           }
        

        printf("</table></td>");

        printf("<td>%s</td>", election.startDate);
        printf("<td>%s</td>", election.endsDate);
        printf("<td>%s</td>", election.resultDate);
        //printf("<td>%s</td>", election.dateOfBirth);
        //printf("<td>%s</td>", election.address);
        printf("</tr>");
    }
    printf("</table>");

    printf("<input type=\"submit\" value=\"Add\" name=\"operation\"/>");
    printf("<input type=\"submit\" value=\"Edit\" name=\"operation\"/>");
    printf("<input type=\"submit\" value=\"Delete\" name=\"operation\"/>");
    //print ends
    printf("</form>");
    printf("</body></html>");
    fclose(fp);
    fclose(fpc);
    return 0;
}