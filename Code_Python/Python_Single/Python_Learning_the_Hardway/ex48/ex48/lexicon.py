"""
direction : north, south, east, west, down, up, left, right, back;
verb : go, stop, kill, eat;
stop : the, in, of, from, at, it;
noun : door, bear, princess, cabinet;
number : 0-9 number
"""
def scan(stuff):
    stuff = stuff.lower()
    words = stuff.split()
    direction = ['north', 'south', 'east', 'west', 'down', 'up', 'left', 'right', 'back']
    verb = ['go', 'stop', 'kill', 'eat']
    stop = ['the', 'in', 'of', 'from', 'at', 'it']
    noun = ['door', 'bear', 'princess', 'cabinet']
    result = [ ]
    for word in words:
        if word in direction:
            result.append(('direction', word))
        elif word in verb:
            result.append(('verb', word))
        elif word in stop:
            result.append(('stop', word))
        elif word in noun:
            result.append(('noun', word))
        elif convert_number(word):
            result.append(('number', int(word)))
        else:
            result.append(('error',word))
    return result 
            
def convert_number(num):
    try:
        return int(num)
    except ValueError:
        return None
                

    
    
    









