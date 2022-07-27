import java.net.URL;
import java.util.ResourceBundle;
import javafx.event.ActionEvent;
import javafx.fxml.*;
import javafx.scene.shape.Circle;
import javafx.scene.control.*;
import javafx.scene.input.MouseEvent;
import java.lang.Math;

public class RobotCarController implements Initializable {
	BTWriter writer = new BTWriter();

	//Constants:
	private final int CENTER_X = 514;
	private final int CENTER_Y = 150;
	private final int MAX_R = 60;

	//JavaFX elements:
	@FXML private Slider kpSlider, kiSlider, kdSlider, speedSlider;
	@FXML private Circle movingCircle, boundaryCircle;
	@FXML private TextArea display;
	@FXML private TextField dataOut;

	//Global variables:
	double[] data = {0, 0, 0, 0, 0, 0};
	String[] labels = {"x-axis: ", "y-axis: ", "kp:      ", "ki:       ", "kd:      ", "speed: "};

	@FXML
	private void moveJoystick(MouseEvent e) {
		double x = e.getSceneX() - CENTER_X;
		double y = e.getSceneY() - CENTER_Y;
		double r = Math.sqrt(x*x + y*y);
		double theta = Math.atan(y/x);

		if (x < 0) theta += Math.PI;

		if (r > MAX_R) {
			r = MAX_R;
			x = r * Math.cos(theta);
			y = r * Math.sin(theta);
		}

		movingCircle.setLayoutX(x + CENTER_X);
		movingCircle.setLayoutY(y + CENTER_Y);

		data[0] = x;
		data[1] = y * (-1);

		updateDisplay();

		writer.write(packData());
	}

	@FXML 
	private void releaseJoystick(MouseEvent e) {
		data[0] = 0;
		data[1] = 0;

		movingCircle.setLayoutX(CENTER_X);
		movingCircle.setLayoutY(CENTER_Y);

		updateDisplay();
		writer.write(packData());
	}

	@FXML
	private void readSlider(MouseEvent e) {
		data[2] = kpSlider.getValue();
		data[3] = kiSlider.getValue();
		data[4] = kdSlider.getValue();
		data[5] = speedSlider.getValue();
		updateDisplay();
	}

	@FXML
	private void releaseSlider(MouseEvent e) {
		writer.write(packData());
	}

	private void updateDisplay() {
		String content = "";
		for (byte i = 0; i < data.length; i++)
			content += String.format("%s %.2f %n", labels[i], data[i]);
		display.setText(content);
	}

	private String packData() {
		String outputString = "<";
		byte i;
		for (i = 0; i < data.length-1; i++)
			outputString += String.format("%.2f", data[i]) + ",";
		outputString += String.format("%.2f", data[i]) + ">";
		dataOut.setText(outputString);
		return outputString;
	}

	@Override 
	public void initialize(URL url, ResourceBundle rb) {
		updateDisplay();
	} 
}