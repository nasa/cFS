import subprocess
import json
import sys
import time
import os

# extract pull request data from GitHub
repos = ['cFS', 'cFE', 'osal', 'psp', 'sch_lab', 'ci_lab', 'to_lab', 'sample_app', 'sample_lib', 'elf2cfetbl', 'tblcrctool','cFS-GroundSystem', 'CF', 'CS', 'DS','FM', 'HK', 'HS', 'LC', 'MD', 'MM', 'SC']

for repo in repos:
    subprocess.Popen('gh pr list --repo nasa/' + str(repo) + ' --search "draft:false" --json number,author,title,url,additions,deletions,labels | jq -c ' + '\'reduce range(0, length) as $index (.; (.[$index].author=.[$index].author.login | .[$index].number=(.[$index].number|"\'' + str(repo) + '\' PR #\(.)") )) | .[]\' ' + '>> temp.json', shell=True)

time.sleep(5)
subprocess.Popen('jq -s . temp.json > prs.json', shell=True)
time.sleep(5)

# load a list of pull requests as python dictionaries
with open ('prs.json') as prs_file:
    prs = json.load(prs_file)

PrData          = dict()    # {author: [pr1, pr2, pr3, ...]}
AuthorPrChanges = dict()    # {author: #TotalChanges}

for pr in prs:
    ignore = False
    for label in pr['labels']:
        if label['name'] == 'CCB:Ignore' or label['name'] == 'CCB:Approved':
            ignore = True
            break
    if ignore == False:
        if pr['author'] not in PrData:
            PrData[pr['author']] = [pr]
            AuthorPrChanges[pr['author']] = pr['additions'] + pr['deletions']
        else:
            PrData[pr['author']].append(pr)
            AuthorPrChanges[pr['author']] += pr['additions'] + pr['deletions']

# no prs to write, exit program
if len(PrData) == 0:
    print("Failed to find relevant Pull Requests for the agenda. Exiting...\n")
    sys.exit()

# re-order dict according to sum(additions, deletions) of each pr for each author
AuthorPrChanges = {k: v for k, v in sorted(AuthorPrChanges.items(), key=lambda item: item[1])}

# write to markdown
CCBfilename = "CCBAgenda.md"
with open(CCBfilename, 'w') as f:
    f.write("## Items for Discussion\n\n")
    for author in AuthorPrChanges.keys():
        f.write("### @" + author + "\n\n")
        for pr_auth in PrData[author]:
            if (author == pr_auth['author']):
                f.write("[" + pr_auth['number'] + "](" + pr_auth['url'].replace("pull", "issues") + ") " + pr_auth['title'] + "\n\n")

# close files
f.close()
prs_file.close()
time.sleep(5)
try:
    os.remove("prs.json")
    os.remove("temp.json")
except OSError:
    pass

time.sleep(5)

if (os.stat(CCBfilename).st_size != 0):
    print("CCB markdown has been successfully created")