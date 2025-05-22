from hugchat import hugchat
from hugchat.login import Login
import sys

if __name__ == '__main__':
    # python3 <filename> API_KEY paragraph_0.txt paragraph_1.txt paragraph_2.txt query.txt
    # print(sys.argv)
    # print(len(sys.argv))
    if len(sys.argv) < 5:
        print("Usage: python3 hug_call.py paragraph_0.txt paragraph_1.txt paragraph_2.txt query.txt")
        sys.exit(1)

    sign = Login("hugging face mail id", "password of hugging face")
    cookies = sign.login()

    # Save cookies to the local directory
    cookie_path_dir = "./cookies_snapshot"
    sign.saveCookiesToDir(cookie_path_dir)

    # Load cookies when you restart your program:
    # sign = login(email, None)
    # cookies = sign.loadCookiesFromDir(cookie_path_dir) # This will detect if the JSON file exists, return cookies if it does and raise an Exception if it's not.

    # Create a ChatBot
    chatbot = hugchat.ChatBot(cookies=cookies.get_dict())  # or cookie_path="usercookies/<email>.json"

    # non stream response

    # Read the paragraphs from the files
    paragraphs = []

    for i in range(2, len(sys.argv)-1):
        with open(sys.argv[i], 'r') as f:
            paragraphs.append(f.read())

    # convert paragraphs to a single string
    paragraphs = '\n'.join(paragraphs)
    query_result = chatbot.query(paragraphs)
    print("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx")
    print("HugBot: ",query_result) # or query_result.text or query_result["text"]
    sourceFile = open(sys.argv[len(sys.argv)-1], 'a')
    print(query_result, file = sourceFile)
    sourceFile.close()
# Log in to huggingface and grant authorization to huggingchat

# stream response
# for resp in chatbot.query(
#     "Hello",
#     stream=True
# ):
#     print(resp)

# Use web search (new feature)
# query_result = chatbot.query("Hi!", web_search=True)
# print(query_result) # or query_result.text or query_result["text"]
# for source in query_result.web_search_sources:
#     print(source.link)
#     print(source.title)
#     print(source.hostname)

# Create a new conversation
# id = chatbot.new_conversation()
# chatbot.change_conversation(id)

# Get conversation list
# conversation_list = chatbot.get_conversation_list()

# Get the available models (not hardcore)
# models = chatbot.get_available_llm_models()

# Switch model to the given index
# chatbot.switch_llm(0) # Switch to the first model
# chatbot.switch_llm(1) # Switch to the second model

# Get information about the current conversation
# info = chatbot.get_conversation_info()
# print(info.id, info.title, info.model, info.system_prompt, info.history)

# Get conversations on the server that are not from the current session (all your conversations in huggingchat)
# chatbot.get_remote_conversations(replace_conversation_list=True)

# # [DANGER] Delete all the conversations for the logged in user
# chatbot.delete_all_conversations()
