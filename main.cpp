#include <gtk/gtk.h>
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <time.h>

using namespace std;

const float PERCENTILE_MARK = 70.8;

struct run_data_container
{
   GObject *label;
   GObject *progress;
};

int percentileScore(float secs)
{
   return round(PERCENTILE_MARK / secs);
}

static double runTest(GtkWidget *widget, run_data_container *data)
{
   clock_t start = clock();
   for(int i = 0; i <= 100000000; ++i)
      sqrt(rand());
   clock_t end = clock();
   float seconds = (float)(end - start) / CLOCKS_PER_SEC;

   string percentile = to_string(percentileScore(seconds));
   string tmp_label;
   if(percentileScore(seconds) % 10 == 2)
      tmp_label = "Your CPU scores in the " + percentile + "nd percentile.";
   else if(percentileScore(seconds) % 10 == 3)
      tmp_label = "Your CPU scores in the " + percentile + "rd percentile.";
   else if(percentileScore(seconds) % 10 == 1)
      tmp_label = "Your CPU scores in the " + percentile + "st percentile.";
   else
      tmp_label = "Your CPU scores in the " + percentile + "th percentile.";
   const char *new_label = tmp_label.c_str();

   gtk_label_set_text(GTK_LABEL(data->label), new_label);
   gtk_button_set_label(GTK_BUTTON(widget), "Rerun");

   return 0;
}

int main(int argc, char *argv[])
{
   // bunch of GObjects
   GtkBuilder *builder;
   GObject *window;
   GObject *main_grid;
   GObject *title_label;
   GObject *subtitle;
   GObject *progress;
   GObject *sub_grid;
   GObject *quit;
   GObject *run;
   GObject *status_label;

   gtk_init(&argc, &argv);

   builder = gtk_builder_new();
   gtk_builder_add_from_file(builder, "alpha.ui", NULL);

   window = gtk_builder_get_object(builder, "window");
   g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

   main_grid      = gtk_builder_get_object(builder, "main_grid");
   sub_grid       = gtk_builder_get_object(builder, "sub_grid");
   run            = gtk_builder_get_object(builder, "run");
   quit           = gtk_builder_get_object(builder, "quit");
   progress       = gtk_builder_get_object(builder, "progress");
   title_label    = gtk_builder_get_object(builder, "title_label");
   subtitle       = gtk_builder_get_object(builder, "subtitle");
   status_label   = gtk_builder_get_object(builder, "status_label");

   run_data_container *rdt = new run_data_container;
   rdt->label = status_label;
   rdt->progress = progress;

   g_signal_connect(quit, "clicked", G_CALLBACK(gtk_main_quit), NULL);
   g_signal_connect(run, "clicked", G_CALLBACK(runTest), rdt);


   gtk_main();

   return 0;
}
