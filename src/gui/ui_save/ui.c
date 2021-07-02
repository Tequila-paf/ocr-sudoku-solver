#include "ui.h"

typedef struct UserInterface
{


} UserInterface;

GtkWidget *window;
GtkImage *image;

// buttons
GtkWidget *load_button;
GtkWidget *binarize_button;
GtkWidget *solve_button;
GtkWidget *rotate_button;
GtkWidget *train_button;
GtkWidget *detect_grid_button;



// entry widgets
GtkWidget *angle_entry;
GtkWidget *eta_entry;
GtkWidget *epoch_entry;


gchar* path;

// text from entry slots
const gchar *angle_text;
const gchar *epoch_text;
const gchar *eta_text;


// Surfaces
SDL_Surface *image_to_process;
SDL_Surface *image_copy;


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
}

void Quit()
{
	gtk_main_quit();
	gtk_widget_destroy(GTK_WIDGET(window));
}

void Solve()
{
	init_sdl();

	if (path == NULL) // could not load img
		return;

	image_to_process = load_image(path);

	if (image_to_process == NULL) // incorrect format
		return;


	image_copy = copy_image(image_to_process);

	PreProcessing(image_to_process);

	int grid_pos_x, grid_pos_y;

	Segmentation(image_to_process, &grid_pos_x, &grid_pos_y);

	SDL_SaveBMP(image_to_process, "processed_img");

	BuildGridToSolve();

	/*
	 * Get grid to solve (path: ../image_processing/grid) -> feed to solver, save result as (../image_processing/grid.result) -> display result
	 */

	SaveResultImage(image_to_process, image_copy, grid_pos_x, grid_pos_y, "../image_processing/grid", "../image_processing/grid.result", "../images/tmp_images/result.bmp");


	Display("../images/tmp_images/result.bmp");

}

void Binarize()
{
//	init_sdl();

	if (path != NULL)
	{
		SDL_Surface *img = load_image(path);
		if (img == NULL)
			return;
		
		grayscale(img);	
		binarize(img);
		SDL_SaveBMP(img, "../images/tmp_images/image_binarized.bmp");

		Display("../images/tmp_images/image_binarized.bmp");

		SDL_FreeSurface(img);
	}	
}

void DetectGrid()
{
//	init_sdl();

	if (path != NULL)
	{
		SDL_Surface *img = copy_image(image_to_process);
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
	angle_text = gtk_entry_get_text(GTK_ENTRY(angle_entry));

	if (image_to_process == NULL)
	{
		g_print("No image to rotate");
		return;
	}

	if (*angle_text == '\0')
	{
		g_print("No entry => Auto Rotate\n");
	}
	else
	{
		double angle = atof(angle_text);

		printf("Angle: %f\n", angle);

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
}



// +-----------------------------------------------Callback funtions---------------------------------------------------+

gboolean on_train()
{
	eta_text = gtk_entry_get_text(GTK_ENTRY(eta_entry));
	epoch_text = gtk_entry_get_text(GTK_ENTRY(epoch_entry));

	double eta = atof(eta_text);
	int nbEpoch = atof(epoch_text);

	//g_print("eta: %f  nb_epoch:%d \n", eta, nbEpoch);

	if (*eta_text != '\0' && *epoch_text != '\0') // no entry
	{
		g_print("Starting training\n");
		TrainNetworkClient(nbEpoch, eta);
		g_print("Finished training\n");
	}
	else
	{
		g_print("No entry\n");
	}
}

gboolean on_rotate()
{
	RotateImage();
	g_print("Rotation\n");
}

gboolean on_detect_grid()
{
	DetectGrid();
}

gboolean on_rotation_entry()
{
	// modify to get rotation angle
	//g_print("on_rotation_entry()\n");
}

gboolean on_eta_entry()
{
	//g_print("on_eta_entry()\n");
}

gboolean on_epoch_entry()
{
	//g_print("on_epoch_entry()\n");
}


gboolean on_load(GtkWidget *widget)
{
	LoadFile(widget);
}

gboolean on_quit(GtkWidget *widget)
{
	Quit();
}

gboolean on_solve(GtkWidget *widget)
{
	Solve();
}

gboolean on_binarize()
{
	Binarize();
}

int main(int argc, char **argv)
{
	init_sdl();
	GtkBuilder *builder;

	gtk_init(NULL, NULL);
	init_sdl();

	builder = gtk_builder_new();

	gtk_builder_add_from_file(builder, "ui.glade", NULL);

	window = GTK_WIDGET(gtk_builder_get_object(builder, "main"));
	gtk_builder_connect_signals(builder, NULL);

	image = GTK_IMAGE(gtk_builder_get_object(builder, "image"));
	load_button = GTK_WIDGET(gtk_builder_get_object(builder, "load_button"));
//	save_button = GTK_WIDGET(gtk_builder_get_object(builder, "save_button"));
	solve_button = GTK_WIDGET(gtk_builder_get_object(builder, "solve_button"));
	angle_entry = GTK_WIDGET(gtk_builder_get_object(builder, "angle_entry"));
	binarize_button = GTK_WIDGET(gtk_builder_get_object(builder, "binarize_button"));
	detect_grid_button = GTK_WIDGET(gtk_builder_get_object(builder, "detect_grid_button"));
	eta_entry = GTK_WIDGET(gtk_builder_get_object(builder, "eta_entry"));
	epoch_entry = GTK_WIDGET(gtk_builder_get_object(builder, "epoch_entry"));
	rotate_button = GTK_WIDGET(gtk_builder_get_object(builder, "rotate_button"));
	train_button = GTK_WIDGET(gtk_builder_get_object(builder, "train_button"));




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

	gtk_main();

	return 0;
}
