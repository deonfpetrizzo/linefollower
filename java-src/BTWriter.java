import java.io.IOException;
import java.io.OutputStream;
import java.io.PrintWriter;

import javax.bluetooth.*;
import javax.microedition.io.Connector;
import javax.microedition.io.StreamConnection;

public class BTWriter {
	private final String HC05_URL = "btspp://98d3a1fd47d9:1;authenticate=false;encrypt=false;master=false";

	public void write(String outputString) {
		Thread thread = new Thread() {
			@Override
			public void run() {
				try {
					StreamConnection connection = (StreamConnection) Connector.open(HC05_URL);
					PrintWriter out = new PrintWriter(connection.openOutputStream());

					out.write(outputString);
					out.close();
					connection.close();
				}
			    	catch (Exception e) {}
			}
		};
		thread.start();
	}
}
