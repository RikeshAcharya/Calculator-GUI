import tkinter as tk

class Calculator:
    def __init__(self, root):
        self.expression = ""
        self.input_text = tk.StringVar()

        root.title("Calculator")
        root.geometry("300x400")
        root.resizable(False, False)

        self.create_widgets(root)

    def create_widgets(self, root):
        # Entry widget
        input_frame = tk.Frame(root)
        input_frame.pack(expand=True, fill="both")

        input_field = tk.Entry(input_frame, font=('arial', 20), textvariable=self.input_text, justify='right', bd=10, relief=tk.RIDGE)
        input_field.pack(expand=True, fill="both")

        # Buttons
        buttons = [
            ["7", "8", "9", "/"],
            ["4", "5", "6", "*"],
            ["1", "2", "3", "-"],
            ["0", "C", "=", "+"]
        ]

        for row in buttons:
            btn_row = tk.Frame(root)
            btn_row.pack(expand=True, fill="both")
            for btn_text in row:
                btn = tk.Button(btn_row, text=btn_text, font=('arial', 18), command=lambda val=btn_text: self.on_button_click(val))
                btn.pack(side="left", expand=True, fill="both")

    def on_button_click(self, char):
        if char == "C":
            self.expression = ""
        elif char == "=":
            try:
                self.expression = str(eval(self.expression))
            except Exception:
                self.expression = "Error"
        else:
            self.expression += str(char)
        self.input_text.set(self.expression)

# Main loop
if __name__ == "__main__":
    root = tk.Tk()
    calc = Calculator(root)
    root.mainloop()
