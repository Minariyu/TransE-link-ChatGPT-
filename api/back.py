from flask import Flask, request, jsonify, render_template
import subprocess

app = Flask(__name__)

@app.route('/')
def index():
    return render_template("web.html")

# 添加内容
@app.route('/add', methods=['POST'])
def add_content():
    data = request.get_json()
    content = data.get('content', '')

    with open("./add.txt", 'a', encoding='utf-8') as f:
        f.write(content + '\n')
    
    return jsonify({'message': 'Content added successfully!'}), 201

# 查看内容
@app.route('/view', methods=['GET'])
def view_content():
    try:
        with open("./add.txt", 'r', encoding='utf-8') as f:
            content = f.read()
        return jsonify({'content': content}), 200
    except FileNotFoundError:
        return jsonify({'content': 'File not found!'}), 404

# 删除内容
@app.route('/delete', methods=['DELETE'])
def delete_content():
    try:
        open("./add.txt", 'w').close()  # Clear file content
        return jsonify({'message': 'Content deleted successfully!'}), 200
    except Exception as e:
        return jsonify({'message': 'Error deleting content: ' + str(e)}), 500

# 重置操作
@app.route('/reset', methods=['POST'])
def reset_content():
    try:
        subprocess.run(['./Train_TransE'], check=True)
        return jsonify({'message': 'Content reset successfully!'}), 200
    except subprocess.CalledProcessError:
        return jsonify({'error': 'Failed to run reset command'}), 500

# 保存问题到 ask.txt
@app.route('/ask', methods=['POST'])
def ask_question():
    data = request.get_json()
    question = data.get('question', '')

    with open('ask.txt', 'a', encoding='utf-8') as f:
        f.write(question + '\n')  # 将问题写入 ask.txt

    try:
        subprocess.run(['./ans'], check=True)
    except subprocess.CalledProcessError:
        return jsonify({'error': 'Failed to run ask program'}), 500
    
    return jsonify({'message': "Now you can view the answer!"}), 200

# 返回 ans.txt 文件内容
@app.route('/get_ans', methods=['GET'])
def get_ans():
    try:
        with open('ans.txt', 'r', encoding='utf-8') as f:
            content = f.read()
        return jsonify({'content': content}), 200
    except FileNotFoundError:
        return jsonify({'content': 'ans.txt not found!'}), 404

if __name__ == '__main__':
    app.run(debug=True, port=5000)