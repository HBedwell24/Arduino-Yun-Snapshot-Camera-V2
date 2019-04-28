# Imports
import base64
import sys

from temboo.Library.Amazon.S3 import PutObject
from temboo.core.session import TembooSession

# Encode image
with open(str(sys.argv[1]), "rb") as image_file:
	encoded_string = base64.b64encode(image_file.read())
	
# Create a session with your Temboo account details
session = TembooSession(USER_NAME, APP_NAME, KEY)

# Instantiate the Choreo
putObjectChoreo = PutObject(session)

# Get an InputSet object for the Choreo
putObjectInputs = putObjectChoreo.new_input_set()

# Set the Choreo inputs
putObjectInputs.set_FileContents(encoded_string)
putObjectInputs.set_BucketName(BUCKET_NAME)
putObjectInputs.set_UserRegion(REGION)
putObjectInputs.set_FileName(IMAGE_NAME)
putObjectInputs.set_AWSAccessKeyId(AWS_ACCESS_KEY)
putObjectInputs.set_AWSSecretKeyId(AWS_SECRET_KEY)

# Execute the Choreo
putObjectResults = putObjectChoreo.execute_with_results(putObjectInputs)

# Print the Choreo outputs
print("Response: " + putObjectResults.get_Response())
