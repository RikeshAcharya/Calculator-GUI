import javax.swing.*;   // for GUI components
import java.awt.*;      // for Layout and containers
import java.awt.event.*; // for ActionListener

// Main calculator class
public class Calculator extends JFrame implements ActionListener {
    private JTextField display;
    private String operator = "";
    private double num1 = 0, num2 = 0;
    private boolean startNew = true;

    public Calculator() {
        setTitle("Calculator");
        setSize(400, 500);
        setDefaultCloseOperation(EXIT_ON_CLOSE);
        setLocationRelativeTo(null);
        setLayout(new BorderLayout());

        display = new JTextField("0");
        display.setEditable(false);
        display.setFont(new Font("Arial", Font.BOLD, 32));
        display.setHorizontalAlignment(JTextField.RIGHT);
        add(display, BorderLayout.NORTH);

        JPanel panel = new JPanel();
        panel.setLayout(new GridLayout(5, 4, 10, 10));

        String[] buttons = {
            "C", "Backspace", "/", "*",
            "7", "8", "9", "-",
            "4", "5", "6", "+",
            "1", "2", "3", "=",
            "0", ".", "", ""
        };

        for (String text : buttons) {
            if (text.equals("")) {
                panel.add(new JLabel());
                continue;
            }
            JButton btn = new JButton(text);
            btn.setFont(new Font("Arial", Font.BOLD, 24));
            btn.addActionListener(this);
            panel.add(btn);
        }

        add(panel, BorderLayout.CENTER);
    }

    @Override
    public void actionPerformed(ActionEvent e) {
        String cmd = e.getActionCommand();

        if (cmd.matches("[0-9]") || cmd.equals(".")) {
            if (startNew || display.getText().equals("0")) {
                display.setText(cmd);
                startNew = false;
            } else {
                display.setText(display.getText() + cmd);
            }
        } else if (cmd.matches("[+\\-*/]")) {
            operator = cmd;
            num1 = Double.parseDouble(display.getText());
            startNew = true;
        } else if (cmd.equals("=")) {
            try {
                num2 = Double.parseDouble(display.getText());
                double result = switch (operator) {
                    case "+" -> num1 + num2;
                    case "-" -> num1 - num2;
                    case "*" -> num1 * num2;
                    case "/" -> {
                        if (num2 == 0)
                            throw new ArithmeticException("Divide by zero");
                        yield num1 / num2;
                    }
                    default -> num2;
                };
                display.setText(String.valueOf(result));
                startNew = true;
            } catch (Exception ex) {
                display.setText("Error");
                startNew = true;
            }
        } else if (cmd.equals("C")) {
            display.setText("0");
            num1 = num2 = 0;
            operator = "";
            startNew = true;
        } else if (cmd.equals("Backspace")) {
            String text = display.getText();
            if (text.length() > 1)
                display.setText(text.substring(0, text.length() - 1));
            else
                display.setText("0");
        }
    }

    public static void main(String[] args) {
        SwingUtilities.invokeLater(() -> {
            Calculator calc = new Calculator();
            calc.setVisible(true);
        });
    }
}
