log = open("debugtrace.out.25700","r")
set= []
for line in log:
    if(line.find("ndp")!=-1):
        if(line.split(" ")[1] not in set):
            set += [line.split(" ")[1]]
print(set)
