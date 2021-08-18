#include "ui.h"

GtkWidget *window;
GtkImage *image;

// buttons
GtkWidget *load_button;
GtkWidget *binarize_button;
GtkWidget *solve_button;
GtkWidget *rotate_button;
GtkWidget *train_button;
GtkWidget *detect_grid_button;
GtkWidget *sobel_button;
GtkWidget *hough_button;
GtkWidget *reset_button;
GtkWidget *save_button;

// entry widgets
GtkWidget *angle_entry;
GtkWidget *eta_entry;
GtkWidget *epoch_entry;
GtkWidget *sobel_entry;
GtkWidget *hough_entry;
GtkWidget *hough_entry_2;


gchar* path;

// text from entry slots
const gchar *angle_text;
const gchar *epoch_text;
const gchar *eta_text;
const gchar *sobel_text;
const gchar *hough_text;
const gchar *hough_text_2;

// Surfaces
SDL_Surface *image_to_process;
SDL_Surface *image_copy;
SDL_Surface *result;


// Functions
void Display(gchar *path)
{
	gtk_image_set_from_file(image, path);
}

void LoadFile(GtkWidget *file_chooser)
{
	path = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(file_chooser));
	g_print("%s\n", path);
	image_to_process = load_image(path);
	image_copy = copy_image(image_to_process);
	Display(path);
	Rescale();
}

void Quit()
{
	remove("../images/tmp_images/auto_rotated.bmp");
	remove("../images/tmp_images/hough.bmp");
	remove("../images/tmp_images/image_binarized.bmp");
	remove("../images/tmp_images/image_grid_detected.bmp");
	remove("../images/tmp_images/image_scaled.bmp");
	remove("../images/tmp_images/rotated_image.bmp");
	remove("../images/tmp_images/sobel.bmp");
	remove("../images/tmp_images/result.bmp");
	gtk_main_quit();
	gtk_widget_destroy(GTK_WIDGET(window));
}

void Solve()
{
	if (path == NULL) // could not load img
		return;

	//image_to_process = load_image(path);

	if (image_to_process == NULL) // incorrect format
		return;


	hough_text_2 = gtk_entry_get_text(GTK_ENTRY(hough_entry_2));

	double hough_threshold = 250;

	if (*hough_text_2 != '\0')
	{
		hough_threshold = atof(hough_text_2);
	}

	image_copy = copy_image(image_to_process);

	PreProcessing(image_to_process);

	int grid_pos_x, grid_pos_y;

	Segmentation(image_to_process, &grid_pos_x, &grid_pos_y, 1.5, hough_threshold);

	// HERE : BUILD RESULT GRID

	BuildGridToSolve();  //unquote when network is ok
	 
	SolveGrid("../image_processing/grid");

	SaveResultImage(image_to_process, image_copy, grid_pos_x, grid_pos_y, "../image_processing/grid", "../image_processing/grid.result", "../images/tmp_images/result.bmp");

	result =  load_image("../images/tmp_images/result.bmp");

	Display("../images/tmp_images/result.bmp");

}

void Binarize()
{
	if (path != NULL)
	{
		SDL_Surface *img = load_image(path);
		if (img == NULL)
			return;
		
		grayscale(img);
		BoxBlur(img);
		BoxBlur(img);	
		binarize(img);
		SDL_SaveBMP(img, "../images/tmp_images/image_binarized.bmp");
		Display("../images/tmp_images/image_binarized.bmp");
		SDL_FreeSurface(img);
	}	
}

void DetectGrid()
{
	if (path != NULL)
	{
		//SDL_Surface *img = copy_image(image_to_process);
		SDL_Surface *img = load_image(path);

		if (img == NULL)
			return;

		SDL_Surface *img_copy = copy_image(img);

		PreProcessing(img);

		int grid_pos_x, grid_pos_y;

		DetectBiggestBlob(img, &grid_pos_x, &grid_pos_y);

		DrawGrid(img, img_copy, grid_pos_x, grid_pos_y);

		SDL_SaveBMP(img_copy, "../images/tmp_images/image_grid_detected.bmp");

		Display("../images/tmp_images/image_grid_detected.bmp");

		SDL_FreeSurface(img);
		SDL_FreeSurface(img_copy);
	}
}

void RotateImage()
{
	// set entry widget
	angle_text = gtk_entry_get_text(GTK_ENTRY(angle_entry));

	if (image_to_process == NULL)
	{
		g_print("No image to rotate");
		return;
	}

	if (*angle_text == '\0') // entry is empty
	{
		g_print("No entry => Auto Rotate\n");
		image_copy = copy_image(image_to_process);
		SDL_Surface *edge_image = SDL_CreateRGBSurface(0, image_copy -> w, image_copy -> h, 32, 0, 0, 0, 0);
		PreProcessing(image_copy);
		SobelEdgeDetection(image_copy, edge_image, 1.5);
		double skew_angle = HoughTransformAngleDetection(edge_image, image_copy, 180, 180, 250, "blue");
		Rotate(image_to_process, skew_angle);
		SDL_SaveBMP(image_to_process, "../images/tmp_images/auto_rotated.bmp");
		Display("../images/tmp_images/auto_rotated.bmp");
		free(edge_image);
	}
	else
	{
		double angle = atof(angle_text);

		printf("Angle: %f\n", angle);

		if (angle < 0)
		{
			angle = 360 - angle;
		}
		Rotate(image_to_process, angle);
	
		Rotate(image_copy, angle);
	}

	SDL_SaveBMP(image_to_process, "../images/tmp_images/rotated_image.bmp");
	Display("../images/tmp_images/rotated_image.bmp");
}

void TrainNetworkClient(int nbEpoch, double eta)
{
	srand(time (NULL));
	struct Network *net = Init();
	net -> eta = eta;

	Train(net, nbEpoch);
	DisplayResultsTrainingSample(net);
	g_print("\n");
}

void Rescale()
{
	if (1)
	{
		cairo_surface_t *cr_surface = cairo_image_surface_create_from_png(path);
		cairo_t *cr = cairo_create(cr_surface);
		cairo_scale(cr, 0.5, 0.5);
		cairo_set_source_surface(cr, cr_surface, 0, 0);
		cairo_surface_write_to_png(cr_surface, "rescaled.png");
	}
}

void Sobel()
{
	if (path != NULL)
	{
		sobel_text = gtk_entry_get_text(GTK_ENTRY(sobel_entry));
		double threshold = atof(sobel_text);

		SDL_Surface *edge_image = SDL_CreateRGBSurface(0, image_to_process -> w, image_to_process -> h, 32, 0, 0, 0, 0);	
		image_copy = copy_image(image_to_process);
		if (*sobel_text != '\0')
		{
			PreProcessing(image_copy);
			SobelEdgeDetection(image_copy, edge_image, threshold);
			SDL_SaveBMP(edge_image, "../images/tmp_images/sobel.bmp");
			Display("../images/tmp_images/sobel.bmp");
		}
		free(edge_image);
	}
}

void Hough()
{
	if (path != NULL)
	{
		hough_text = gtk_entry_get_text(GTK_ENTRY(hough_entry));
		double hough_threshold = atof(hough_text);
		SDL_Surface *img_copy = copy_image(image_to_process);

		if (*hough_text != '\0')
		{
			PreProcessing(img_copy);
			int grid_px, grid_py;
			DetectBiggestBlob(img_copy, &grid_px, &grid_py);
			LineDetection(img_copy, 1.5, hough_threshold, "red");
			SDL_SaveBMP(img_copy, "../images/tmp_images/hough.bmp");
			Display("../images/tmp_images/hough.bmp");
		}
		free(img_copy);
	}
}

void ResetImage()
{
	if (path != NULL)
	{
		image_to_process = load_image(path);
		Display(path);
	}
}

void Save()
{
	if (result != NULL)
	{
		SDL_SaveBMP(result, "grid_result.bmp");
	}
}

// +-----------------------------------------------Callback funtions---------------------------------------------------+

gboolean on_train()
{
	eta_text = gtk_entry_get_text(GTK_ENTRY(eta_entry));
	epoch_text = gtk_entry_get_text(GTK_ENTRY(epoch_entry));

	double eta = atof(eta_text);
	int nbEpoch = atof(epoch_text);

	g_print("eta: %f  nb_epoch:%d \n", eta, nbEpoch);

	if (*eta_text != '\0' && *epoch_text != '\0') // no entry
	{
		g_print("Starting training\n");
		TrainNetworkClient(nbEpoch, eta);
		g_print("Finished training\n");
		
		g_print("\n");
	}
	else
	{
		g_print("No entry\n");
	}
	return FALSE;
}

gboolean on_rotate()
{
	RotateImage();
//	g_print("Rotation\n");
	return FALSE;
}

gboolean on_detect_grid()
{
	DetectGrid();
	return FALSE;
}

gboolean on_rotation_entry()
{
	// modify to get rotation angle
	//g_print("on_rotation_entry()\n");
	return FALSE;
}

gboolean on_eta_entry()
{
//	g_print("on_eta_entry()\n");
	return FALSE;
}

gboolean on_epoch_entry()
{
//	g_print("on_epoch_entry()\n");
	return FALSE;
}

gboolean on_sobel_entry()
{
	g_print("on_sobel_entry()\n");
	return FALSE;
}

gboolean on_hough_entry()
{
	g_print("on_hough_entry()\n");
	return FALSE;
}


gboolean on_load(GtkWidget *widget)
{
	LoadFile(widget);
	return FALSE;
}

gboolean on_quit(GtkWidget *widget)
{
	Quit();
	return FALSE;
}

gboolean on_solve(GtkWidget *widget)
{
	Solve();
	return FALSE;
}

gboolean on_binarize(GtkWidget *widget)
{
	Binarize();
	return FALSE;
}

gboolean on_sobel(GtkWidget *widget)
{
	Sobel();
	return FALSE;
}	

gboolean on_hough(GtkWidget *widget)
{
	Hough();
	return FALSE;
}

gboolean on_reset(GtkWidget *widget)
{
	ResetImage();
	return FALSE;
}

gboolean on_save(GtkWidget *widget)
{
	Save();
	return FALSE;
}

int LaunchUserInterface(int argc, char **argv)
{
	init_sdl();

	gtk_init(NULL, NULL);

	GtkBuilder *builder = gtk_builder_new();

	GError *error = NULL;

	if (gtk_builder_add_from_file(builder, "ui.glade", &error) == 0)
	{
		g_printerr("Error loading file :%s\n", error -> message);
		g_clear_error(&error);
		return 1;
	}

	window = GTK_WIDGET(gtk_builder_get_object(builder, "main"));
	gtk_builder_connect_signals(builder, NULL);

	image = GTK_IMAGE(gtk_builder_get_object(builder, "image"));
	load_button = GTK_WIDGET(gtk_builder_get_object(builder, "load_button"));
	solve_button = GTK_WIDGET(gtk_builder_get_object(builder, "solve_button"));
	angle_entry = GTK_WIDGET(gtk_builder_get_object(builder, "angle_entry"));
	binarize_button = GTK_WIDGET(gtk_builder_get_object(builder, "binarize_button"));
	detect_grid_button = GTK_WIDGET(gtk_builder_get_object(builder, "detect_grid_button"));
	eta_entry = GTK_WIDGET(gtk_builder_get_object(builder, "eta_entry"));
	epoch_entry = GTK_WIDGET(gtk_builder_get_object(builder, "epoch_entry"));
	rotate_button = GTK_WIDGET(gtk_builder_get_object(builder, "rotate_button"));
	train_button = GTK_WIDGET(gtk_builder_get_object(builder, "train_button"));
	sobel_button = GTK_WIDGET(gtk_builder_get_object(builder, "sobel_button"));
	sobel_entry = GTK_WIDGET(gtk_builder_get_object(builder, "sobel_entry"));
	hough_button = GTK_WIDGET(gtk_builder_get_object(builder, "hough_button"));
	hough_entry = GTK_WIDGET(gtk_builder_get_object(builder, "hough_entry"));
	hough_entry_2 = GTK_WIDGET(gtk_builder_get_object(builder, "hough_entry_2"));
	reset_button = GTK_WIDGET(gtk_builder_get_object(builder, "reset_button"));
	save_button = GTK_WIDGET(gtk_builder_get_object(builder, "save_button"));


	g_signal_connect(load_button, "file-set", G_CALLBACK(on_load), NULL);
	g_signal_connect(window, "destroy", G_CALLBACK(on_quit), NULL);
	g_signal_connect(solve_button, "clicked", G_CALLBACK(on_solve), NULL);
	g_signal_connect(binarize_button, "clicked", G_CALLBACK(on_binarize), NULL);
	g_signal_connect(angle_entry, "changed", G_CALLBACK(on_rotation_entry), NULL);
	g_signal_connect(detect_grid_button, "clicked", G_CALLBACK(on_detect_grid), NULL);
	g_signal_connect(train_button, "clicked", G_CALLBACK(on_train), NULL);
	g_signal_connect(eta_entry, "changed", G_CALLBACK(on_eta_entry), NULL);
	g_signal_connect(epoch_entry, "changed", G_CALLBACK(on_epoch_entry), NULL);
	g_signal_connect(rotate_button, "clicked", G_CALLBACK(on_rotate), NULL);
	g_signal_connect(sobel_button, "clicked", G_CALLBACK(on_sobel), NULL);
	g_signal_connect(sobel_entry, "changed", G_CALLBACK(on_sobel_entry), NULL);
	g_signal_connect(hough_button, "clicked", G_CALLBACK(on_hough), NULL);
	g_signal_connect(hough_entry, "changed", G_CALLBACK(on_hough_entry), NULL);
	g_signal_connect(hough_entry_2, "changed", G_CALLBACK(on_hough_entry), NULL);
	g_signal_connect(reset_button, "clicked", G_CALLBACK(on_reset), NULL);
	g_signal_connect(save_button, "clicked", G_CALLBACK(on_save), NULL);

	gtk_main();

}

int main(int argc, char **argv)
{
	LaunchUserInterface(argc, argv);
	return 0;
}

