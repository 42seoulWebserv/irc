import os
import sys
import json
import fcntl
import random

id = None
sessionFilePath = 'sessions'

def getSessionId():
    global id
    if id is not None:
        return id
    id = ''.join(random.sample('abcdefghijklmnopqrstuvwxyz0123456789', 8))
    if 'HTTP_COOKIE' not in os.environ:
        return id
    cookies = os.environ['HTTP_COOKIE'].split(';')
    for cookie in cookies:
        if cookie.startswith('session='):
            id = cookie[8:]
            return id
    return id

def getSesseionData():
    sessionId = getSessionId()
    if not os.path.exists(sessionFilePath):
        open(sessionFilePath, 'w').close()
    with open(sessionFilePath, 'r+') as file:
        fcntl.flock(file.fileno(), fcntl.LOCK_EX)
        content = file.read()
        if content == '':
            sessions = {}
        else:
            sessions = json.loads(content)
        fcntl.flock(file.fileno(), fcntl.LOCK_UN)
        if sessionId not in sessions:
            return []
        return sessions[sessionId]

def setSessionData(data):
    sessionId = getSessionId()
    if not os.path.exists(sessionFilePath):
        open(sessionFilePath, 'w').close()
    with open('sessions', 'r+') as file:
        fcntl.flock(file.fileno(), fcntl.LOCK_EX)
        content = file.read()
        if content == '':
            sessions = {}
        else:
            sessions = json.loads(content)
        sessions[sessionId] = data
        file.seek(0)
        file.write(json.dumps(sessions))
        file.truncate()
        fcntl.flock(file.fileno(), fcntl.LOCK_UN)

def writeResonse(status, body):
    sys.stdout.write("Status: %d\r\n" % status)
    sys.stdout.write("Content-Length: %d\r\n" % len(body))
    if 'HTTP_ORIGIN' not in os.environ:
        sys.stdout.write("Set-Cookie: session=%s\r\n" % getSessionId())
    sys.stdout.write("Content-Type: application/json\r\n\r\n")
    sys.stdout.write(body)

if 'REQUEST_METHOD' not in os.environ:
    sys.exit(1)
method = os.environ['REQUEST_METHOD']

if 'CONTENT_LENGTH' not in os.environ:
    sys.exit(1)
bodyLen = int(os.environ["CONTENT_LENGTH"])
body = sys.stdin.buffer.read(bodyLen).decode()


if method == 'GET':
    goods = getSesseionData()
    writeResonse(200, json.dumps(goods))

elif method == 'POST':
    goods = getSesseionData()
    goods.append(body)
    setSessionData(goods)
    writeResonse(201, '')

elif method == 'PUT':
    goods = getSesseionData()
    if body in goods:
        writeResonse(200, '')
    else:
        goods.append(body)
        setSessionData(goods)
        writeResonse(201, '')

elif method == 'DELETE':
    goods = getSesseionData()
    if body in goods:
        goods.remove(body)
        setSessionData(goods)
        writeResonse(200, '')
    else:
        writeResonse(204, '')

else:
    sys.exit(1)

