#include "opencv2/opencv.hpp"

#include <cstdio>
#include <iostream>

/// Process frame and convert it to ASCII output into terminal.
/// NOTE: Provides no safety checks for appropriate output str size.
void
cv_mat_to_ascii_output(cv::Mat& frame,
                       std::string& output,
                       double const RESIZE_SCALE = 0.125)
{
    // Map that associates some pixel value range to a corresponding ASCII
    // character ordered from highest density -> lowest density.
    static char constexpr PIXEL_TO_ASCII_MAP[] =
      "$@B%8&WM#*oahkbdpqwmZO0QLCJUYXzcvunxrjft/\\|()1{}[]?-_+~<>i!lI;:,\"^`'. "
      "'`";
    static auto constexpr ASCII_MAP_SIZE = sizeof(PIXEL_TO_ASCII_MAP);

    // Resize frame
    cv::resize(frame,
               frame,
               cv::Size(),
               RESIZE_SCALE * 1.5,
               RESIZE_SCALE,
               cv::INTER_AREA);

    // Convert to greyscale
    cv::cvtColor(frame, frame, cv::COLOR_BGR2GRAY);

    auto output_idx = 0;
    for (auto i = 0; i < frame.rows; ++i) {
        for (auto j = 0; j < frame.cols; ++j) {
            // Normalize greyscale pixel value (0-255) into an index for
            // the pixel ascii map.
            output[output_idx++] =
              PIXEL_TO_ASCII_MAP[static_cast<size_t>(frame.at<uint8_t>(i, j)) *
                                 ASCII_MAP_SIZE / 256];
        }
        output[output_idx++] = '\n';
    }

    std::printf("%s", output.c_str());
}

// Returns an appropriately sized output string for ASCII representation
// of a frame, given a RESIZE_SCALE factor.
std::string
generate_output_str_for_frame(cv::Mat& frame, double const RESIZE_SCALE = 0.125)
{
    cv::resize(frame,
               frame,
               cv::Size(),
               RESIZE_SCALE * 1.5,
               RESIZE_SCALE,
               cv::INTER_AREA);

    // (HEIGHT * WIDTH pixels) + (HEIGHT '\n' characters)
    return std::string(frame.rows * frame.cols + frame.rows, '\0');
}

int
main()
{
    auto vidcap = cv::VideoCapture(0);
    if (!vidcap.isOpened()) {
        std::cerr << "Error opening video capture" << std::endl;
        return 1;
    }

    // Process and resize first frame so we can size our
    // output string accordingly only once.
    auto constexpr RESIZE_SCALE = 0.125;
    auto frame = cv::Mat();
    vidcap >> frame;
    if (frame.empty()) {
        std::cerr << "Error capturing initial frame." << std::endl;
        return 1;
    }
    auto output = generate_output_str_for_frame(frame, RESIZE_SCALE);

    // Capture loop
    while (true) {
        vidcap >> frame;
        if (frame.empty())
            break;

        cv_mat_to_ascii_output(frame, output, RESIZE_SCALE);

        // ctrl-c signal
        if (static_cast<char>(cv::waitKey(1)) == 1)
            break;
    }

    if (vidcap.isOpened())
        vidcap.release();

    cv::destroyAllWindows();
}
