import os

def dfs(pid):
    print(pid)
    try:
        with open(f"/proc/{pid}/task/{pid}/children", 'r') as f:
            children = f.read().strip()
    except IOError:
        return
    
    if children:
        for child_pid in children.split():
            dfs(int(child_pid))

dfs(1)

