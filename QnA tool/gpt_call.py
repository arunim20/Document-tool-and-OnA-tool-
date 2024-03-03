import openai
import sys


if __name__ == '__main__':
    if len(sys.argv) < 5:
        print("Usage: python3 gpt_call.py API_KEY paragraph_0.txt paragraph_1.txt paragraph_2.txt query.txt")
        sys.exit(1)

    # Read the API key from the command line
    openai.api_key = sys.argv[1]

    # Read the paragraphs from the files
    paragraphs = []

    for i in range(2, len(sys.argv)-1):
        with open(sys.argv[i], 'r') as f:
            paragraphs.append(f.read())

    # convert paragraphs to a single string
    paragraphs = '\n'.join(paragraphs)

    query = {
        "role": "user", "content": paragraphs
    }

    chat = openai.ChatCompletion.create(
        model="gpt-3.5-turbo",
        messages=[query]
    )

    reply = chat.choices[0].message.content
    print("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx")
    print("ChatGPT: ",reply)
    sourceFile = open(sys.argv[len(sys.argv)-1], 'a')
    print(reply, file = sourceFile)
    sourceFile.close()

