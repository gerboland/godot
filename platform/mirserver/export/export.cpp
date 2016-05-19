#include "export.h"
#include "platform/mirserver/logo.h"
#include "tools/editor/editor_import_export.h"
#include "scene/resources/texture.h"

void register_mirserver_exporter() {

	Image img(_mirserver_logo);
	Ref<ImageTexture> logo = memnew( ImageTexture );
	logo->create_from_image(img);

	{
		Ref<EditorExportPlatformPC> exporter = Ref<EditorExportPlatformPC>( memnew(EditorExportPlatformPC) );
		exporter->set_binary_extension("");
		exporter->set_release_binary32("linux_mirserver_32_release");
		exporter->set_debug_binary32("linux_mirserver_32_debug");
		exporter->set_release_binary64("linux_mirserver_64_release");
		exporter->set_debug_binary64("linux_mirserver_64_debug");
		exporter->set_name("Linux MirServer");
		exporter->set_logo(logo);
		EditorImportExport::get_singleton()->add_export_platform(exporter);
	}

}
