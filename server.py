import telebot
from flask import Flask, request, render_template


app = Flask(__name__)
bot = telebot.TeleBot("2010829751:AAF2GXHZMPIcfXuAsCGOviH0-3Ushubvo1w")


@app.route("/")
def home():
	return render_template("index.html")

@app.route("/api")
def api():
	data = request.args.get('data')


app.run(debug=True)