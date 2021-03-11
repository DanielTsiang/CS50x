import os

from cs50 import SQL
from flask import Flask, flash, jsonify, redirect, render_template, request, session

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///birthdays.db")

@app.route("/", methods=["GET", "POST"])
def index():
    if request.method == "POST":
        # Access form data
        name = request.form.get("name")
        month = request.form.get("month")
        day = request.form.get("day")

        # Insert data into database
        db.execute("INSERT INTO birthdays (name, month, day) VALUES (?, ?, ?)", name, month, day)

        # Go back to homepage
        return redirect("/")

    else:
        # Query for all birthdays
        birthdays = db.execute("SELECT * FROM birthdays")

        # Render birthdays page
        return render_template("index.html", birthdays=birthdays)

@app.route("/delete", methods=["POST"])
def delete():
    # Access form data
    id = request.form.get("id")

    # Delete data from database
    db.execute("DELETE FROM birthdays WHERE id = ?", id)

    # Go back to homepage
    return redirect("/")

@app.route("/edit", methods=["POST"])
def edit():
    # Access form data
    id = request.form.get("EditId")
    name = request.form.get("EditName")
    day = request.form.get("EditDay")
    month = request.form.get("EditMonth")

    # Update data in database
    db.execute("UPDATE birthdays SET name = ?, day = ?, month = ? WHERE id = ?", name, day, month, id)

    # Go back to homepage
    return redirect("/")
