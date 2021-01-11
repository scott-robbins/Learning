import base64
from flask import Flask
from flask_mail import Mail, Message

mail = Mail()

app = Flask(__name__)
mail_settings = {
    "MAIL_SERVER": "smtp.mail.yahoo.com",
    "MAIL_PORT": 465,
    "MAIL_USE_TLS": False,
    "MAIL_USE_SSL": True,
    "MAIL_USERNAME": "dixhappy69@yahoo.com",
    "MAIL_PASSWORD": "imgqrvsyuojkebhx",
}

app.config.update(mail_settings)
mail.init_app(app)


@app.route("/")
def index():
    msg = Message(
        "Flask mail test",
        sender="dixhappy69@yahoo.com",
        recipients=["tyl3r5durd3n@gmail.com"]
    )
    msg.body = "Hey Fuckface, can you see this? :D"
    msg.html = "<b>testing</b>"
    status = mail.send(msg)
    return str(status)


if __name__ == "__main__":
    app.run()