#include "UI.h"
#include <unordered_map>

void UI::Run() {
    while (true) {
        const string filter_prompt = "Available Filters:\n"
                                     "1) Grayscale\n2) Black-and-White\n3) Invert\n"
                                     "4) Merge\n5) Flip\n6) Rotate\n"
                                     "7) Lighten/Darken\n8) Crop\n9) Frame\n"
                                     "10) Detect Edges\n11) Resize\n12) Blur\n"
                                     "13) Sunlight\n14) Oil Painting\n15) Old TV\n"
                                     "16) Quit\n\nChoose a filter (1-16): ";

        const int choice = Utilities::v_numericalInput(filter_prompt, 1, 16);
        if (choice == QUIT) break;

        // Common image loading
        auto load_images = [this](int choice) -> pair<Image, Image> {
            string prompt = "Enter filename of " +
                            string(choice == MERGE ? "first " : "") + "image: ";
            string fname = Utilities::get_valid_image_filename(prompt, true);
            Image img1 = load_image(fname);

            Image img2;
            if (choice == MERGE) {
                prompt = "Enter second image filename: ";
                fname = Utilities::get_valid_image_filename(prompt, true);
                img2 = load_image(fname);
            }
            return {img1, img2};
        };

        auto [input1, input2] = load_images(choice);

        // Parameters collection
        FilterParams params;
        const Image& ref = input1;  // Reference for dimensions

        auto get_merge_params = [&] {
            params.resize_or_not = Utilities::v_numericalInput(
                    "Merge method (0: Crop, 1: Resize): ", 0, 1);
        };

        auto get_flip_params = [&] {
            params.direction = Utilities::v_numericalInput(
                    "Flip direction (1: Horizontal, 2: Vertical): ", 1, 2);
        };

        auto get_rotate_params = [&] {
            params.angle = Utilities::v_numericalInput(
                    "Rotation angle (90/180/270): ", {90, 180, 270});
        };

        auto get_crop_params = [&] {
            params.vertex_row = Utilities::v_numericalInput(
                    "Start row (1-" + to_string(ref.height) + "): ", 1, ref.height) - 1;
            params.vertex_col = Utilities::v_numericalInput(
                    "Start column (1-" + to_string(ref.width) + "): ", 1, ref.width) - 1;
            params.crop_h = Utilities::v_numericalInput(
                    "Height (1-" + to_string(ref.height - params.vertex_row) + "): ", 1);
            params.crop_w = Utilities::v_numericalInput(
                    "Width (1-" + to_string(ref.width - params.vertex_col) + "): ", 1);
        };

        const unordered_map<int, function<void()>> param_handlers = {
                {MERGE, [&] {
                    params.resize_or_not = Utilities::v_numericalInput(
                            "Merge method (0: Crop, 1: Resize): ", 0, 1);
                }},
                {FLIP, [&] {
                    params.direction = Utilities::v_numericalInput(
                            "Flip direction (1: Horizontal, 2: Vertical): ", 1, 2);
                }},
                {ROTATE, [&] {
                    params.angle = Utilities::v_numericalInput(
                            "Rotation angle (90/180/270): ", vector<int>{90, 180, 270});
                }},
                {LIGHTEN, [&] {
                    params.lighten = Utilities::v_numericalInput(
                            "Lighten (1) or darken (0): ", 0, 1);
                }},
                {CROP, [&] {
                    params.vertex_row = Utilities::v_numericalInput(
                            "Start row (1-" + to_string(input1.height) + "): ",
                            1, input1.height) - 1;
                    params.vertex_col = Utilities::v_numericalInput(
                            "Start column (1-" + to_string(input1.width) + "): ",
                            1, input1.width) - 1;
                    params.crop_h = Utilities::v_numericalInput(
                            "Height (1-" + to_string(input1.height - params.vertex_row) + "): ", 1);
                    params.crop_w = Utilities::v_numericalInput(
                            "Width (1-" + to_string(input1.width - params.vertex_col) + "): ", 1);
                }},
                {FRAME, [&] {
                    params.fancy = Utilities::v_numericalInput(
                            "Frame type (0: Simple, 1: Fancy): ", 0, 1);
                    params.color = Utilities::v_numericalInput(
                            "Color (1-7): \n1) Black 2) White 3) Brown\n"
                            "4) Red 5) Green 6) Blue 7) Yellow: ", 1, 7);
                }},
                {RESIZE, [&] {
                    params.new_w = Utilities::v_numericalInput("Enter new width: ", 1);
                    params.new_h = Utilities::v_numericalInput("Enter new height: ", 1);
                }},
                {BLUR, [&] {
                    params.blur_radius = Utilities::v_numericalInput(
                            "Blur intensity (1-5): ", 1, 5);
                }},
                {SUNLIGHT, [&] {
                    params.sun_intensity = Utilities::v_numericalInput(
                            "Sunlight intensity (1-100): ", 1, 100);
                }},
                {OIL, [&] {
                    params.brush_size = Utilities::v_numericalInput(
                            "Brush size (3-15 odd): ", 3, 15);
                }},
                {TV, [&] {
                    params.noise_intensity = Utilities::v_numericalInput(
                            "Static intensity (1-10): ", 1, 10);
                }}
        };

        if (param_handlers.count(choice)) {
            param_handlers.at(choice)();
        }

        // Output dimensions calculation
        auto [out_w, out_h] = [&]() -> pair<int, int> {
            switch (choice) {
                case MERGE:
                    return params.resize_or_not ?
                           make_pair(max(input1.width, input2.width), max(input1.height, input2.height)) :
                           make_pair(min(input1.width, input2.width), min(input1.height, input2.height));
                case ROTATE:
                    return (params.angle % 180) ?
                           make_pair(input1.height, input1.width) : make_pair(input1.width, input1.height);
                case CROP:
                    return {params.crop_w, params.crop_h};
                case RESIZE:
                    return {params.new_w, params.new_h};
                default:
                    return {input1.width, input1.height};
            }
        }();

        // Apply filter
        Image output(out_w, out_h);
        cout << "\nApplying filter...\n";


        const unordered_map<int, function<void()>> filter_appliers = {
                {GRAYSCALE, [&]{ Filters::grayScale(input1, output); }},
                {BW, [&]{ Filters::Black_White(input1, output); }},
                {INVERT, [&]{ Filters::invert(input1, output); }},
                {MERGE, [&]{ Filters::merge(input1, input2, output, params.resize_or_not); }},
                {FLIP, [&]{ Filters::flip(input1, output, params.direction); }},
                {ROTATE, [&]{ Filters::rotate(input1, output, params.angle); }},
                {LIGHTEN, [&]{ Filters::exposure(input1, output, params.lighten); }},
                {CROP, [&]{ Filters::crop(input1, output, params.vertex_row, params.vertex_col); }},
                {FRAME, [&]{ Filters::frame(input1, output, params.fancy, params.color, UI::COLOR_MAP); }},
                {EDGES, [&]{ Filters::edges(input1, output); }},
                {RESIZE, [&]{ Filters::resize(input1, output); }},
                {SUNLIGHT, [&]{
                    // Original signature: sunlight(Image, Image)
                    Filters::sunlight(input1, output);
                }},
                {OIL, [&]{
                    // Original signature: oilPainting(Image, Image)
                    Filters::oilPainting(input1, output);
                }},
                {TV, [&]{
                    // Original signature: oldTV(Image, Image)
                    Filters::oldTV(input1, output);
                }},
                {BLUR, [&]{
                    // Maintain original blur logic with calculated values
                    Filters::blur(input1, output,
                                  sqrt((input1.height * input1.width))/160, 1);
                    Filters::blur(output, output,
                                  sqrt((input1.height * input1.width))/80, 2);
                }}
        };

        if (filter_appliers.count(choice)) {
            filter_appliers.at(choice)();
        }

        save_image(output);
        cout << "\n" << string(80, '-') << "\n";
    }
}