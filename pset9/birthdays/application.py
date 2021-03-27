import os

from cs50 import SQL
from flask import Flask, flash, jsonify, redirect, render_template, request, session
import json
from datetime import datetime

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True

# Custom filter for datetimeformat
def datetimeformat(dt_string, format="%d-%m-%Y"):
    dt_object = datetime.strptime(dt_string, "%Y-%m-%d")
    return dt_object.strftime(format)

# Custom filters
app.jinja_env.filters["datetimeformat"] = datetimeformat

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///birthdays.db", connect_args={"check_same_thread":False})

@app.route("/", methods=["GET", "POST"])
def index():
    if request.method == "POST":
        # Access form data
        name = request.form.get("name")
        birthday = datetimeformat(request.form.get("birthday"))

        # Insert data into database
        db.execute("INSERT INTO birthdays (name, birthday) VALUES (?, ?)", name, birthday)

        # Return success status
        return jsonify(True)

    else:
        # Render birthdays page
        return render_template("index.html")

@app.route("/data", methods=["GET"])
def data():
    # Query for all birthdays
    birthdays = db.execute("SELECT * FROM birthdays")

    # Return birthdays data as JSON
    return jsonify({"data": birthdays})

@app.route("/delete", methods=["POST"])
def delete():
    # Access form data
    id = request.form.get("id")

    # Delete data from database
    db.execute("DELETE FROM birthdays WHERE id = ?", id)

    # Return success status
    return jsonify(True)

@app.route("/update", methods=["POST"])
def update():
    # Access form data
    id = request.form.get("UpdateId")
    name = request.form.get("UpdateName")
    get_birthday = request.form.get("UpdateBirthday")

    # user did not update birthday
    if not get_birthday:
        birthday = get_birthday
    # user updated birthday
    else:
        birthday = datetimeformat(get_birthday)

    # Select old data from database row
    old = db.execute("SELECT * FROM birthdays WHERE id = ?", id)[0]

    # Update data in database row for new values given by user, keep old data if no new value given
    db.execute("UPDATE birthdays SET name = coalesce(NULLIF(?, ''), ?), birthday = coalesce(NULLIF(?, ''), ?) WHERE id = ?",
    name, old["name"], birthday, old["birthday"], id)

    # Return success status
    return jsonify(True)
