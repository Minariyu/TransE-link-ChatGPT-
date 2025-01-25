import os
from openai import OpenAI

# 创建 OpenAI 客户端
client = OpenAI(
	api_key="sk-B13g8LgdupFlKMhs20799753Ca0d4231B715E481Dc2257Bb",
	base_url="https://free.v36.cm/v1/"
)

def gpt_35_api(message: dict):
    """非流式响应"""
    try:
        completion = client.chat.completions.create(model="gpt-3.5-turbo", messages=[message])
        response = completion.choices[0].message.content
        return response
    except Exception as e:
        print(f"错误: {e}")
        return None

def contains_yes_no(response: str) -> str:
	"""判断响应中是否包含 'yes' 或 'no'"""
	if 'yes' in response.lower():
		return 2
	elif 'no' in response.lower():
		return 1
	else:
		return 0

if __name__ == '__main__':
	filepath = 'question.txt'  # 指定输入文件路径
	output_filepath = 'output.txt'
	cnt=-1
	with open(filepath, 'r', encoding='utf-8') as file:
		for line in file:
			cnt=cnt+1
			print(cnt)
			word=line.strip().split()
			if len(word)<8:
				continue
			while 1:
				message={'role': 'user', 'content': line.strip()}
				response = gpt_35_api(message)
				if response :
					result = contains_yes_no(response)
					with open(output_filepath, 'a', encoding='utf-8') as output_file:
						if cnt%11==0:
							output_file.write(f"{word[1]} {word[2]} {word[3]} 1\n")
						else :
							if result == 1:
								output_file.write(f"{word[1]} {word[2]} {word[3]} 0\n")
					break
				else :
					print("未能获取响应")


# 2730