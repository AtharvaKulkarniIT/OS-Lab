import java.util.Arrays;
import java.util.Scanner;

public class ScanDiskScheduling {

    // SCAN Disk Scheduling Function
    public static int scan(int[] requests, int head, int diskSize, boolean directionRight) {
        Arrays.sort(requests);  // Sort the request array
        int totalSeek = 0;
        int currentPosition = head;

        System.out.println("Disk head movement:");

        // Move to the right
        if (directionRight) {
            // Serve requests on the right of head
            for (int request : requests) {
                if (request >= head) {
                    totalSeek += Math.abs(currentPosition - request);
                    System.out.println("Move from " + currentPosition + " to " + request);
                    currentPosition = request;
                }
            }

            // Move to the end of the disk
            if (currentPosition < diskSize - 1) {
                totalSeek += Math.abs(currentPosition - (diskSize - 1));
                System.out.println("Move from " + currentPosition + " to " + (diskSize - 1));
                currentPosition = diskSize - 1;
            }

            // Reverse direction and serve requests on the left
            for (int i = requests.length - 1; i >= 0; i--) {
                if (requests[i] < head) {
                    totalSeek += Math.abs(currentPosition - requests[i]);
                    System.out.println("Move from " + currentPosition + " to " + requests[i]);
                    currentPosition = requests[i];
                }
            }
        } 
        // Move to the left
        else {
            // Serve requests on the left of head
            for (int i = requests.length - 1; i >= 0; i--) {
                if (requests[i] <= head) {
                    totalSeek += Math.abs(currentPosition - requests[i]);
                    System.out.println("Move from " + currentPosition + " to " + requests[i]);
                    currentPosition = requests[i];
                }
            }

            // Move to the start of the disk
            if (currentPosition > 0) {
                totalSeek += Math.abs(currentPosition - 0);
                System.out.println("Move from " + currentPosition + " to " + 0);
                currentPosition = 0;
            }

            // Reverse direction and serve requests on the right
            for (int request : requests) {
                if (request > head) {
                    totalSeek += Math.abs(currentPosition - request);
                    System.out.println("Move from " + currentPosition + " to " + request);
                    currentPosition = request;
                }
            }
        }
        
        return totalSeek;
    }

    public static void main(String[] args) {
        Scanner sc = new Scanner(System.in);

        System.out.print("Enter the initial head position: ");
        int head = sc.nextInt();

        System.out.print("Enter the disk size: ");
        int diskSize = sc.nextInt();

        System.out.print("Enter the number of requests: ");
        int n = sc.nextInt();
        int[] requests = new int[n];

        System.out.println("Enter the requests:");
        for (int i = 0; i < n; i++) {
            requests[i] = sc.nextInt();
        }

        System.out.print("Enter initial direction (1 for right, 0 for left): ");
        boolean directionRight = sc.nextInt() == 1;

        int totalSeek = scan(requests, head, diskSize, directionRight);
        System.out.println("Total seek time for SCAN: " + totalSeek);

        sc.close();
    }
}
